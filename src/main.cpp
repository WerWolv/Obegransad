#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <hal/button.hpp>
#include <hal/screen.hpp>

#include <screens/screen.hpp>
#include <screens/screen_time.hpp>
#include <screens/screen_game_of_life.hpp>
#include <screens/screen_circle.hpp>
#include <screens/screen_firework.hpp>
#include <screens/screen_stars.hpp>
#include <screens/screen_rain.hpp>

#include <config.hpp>

static void configureWiFi() {
    WiFi.begin(obegransad::config::WiFiSSID, obegransad::config::WiFiPassword);

    while (true) {
        delay(500);

        auto status = WiFi.status();
        if (status == WL_DISCONNECTED) {
            Serial.print(".");
        } else if (status == WL_CONNECTED) {
            Serial.println("WiFi connected");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            break;
        } else if (status == WL_CONNECT_FAILED || status == WL_NO_SSID_AVAIL) {
            Serial.print("WiFi connection failed, status: ");
            Serial.println(status);
        } else {
            Serial.print("Connecting to WiFi, status: ");
            Serial.println(status);
        }
    }

    // Configure NTP
    configTime("CET-1CEST,M3.5.0/2,M10.5.0/3", "0.ch.pool.ntp.org", "0.europe.pool.ntp.org", "time.nist.gov");
}

static void configureOTA() {
    ArduinoOTA.onStart([]() {
         Serial.println("Start\n");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("End\n");
    });

    ArduinoOTA.onProgress([](std::uint32_t progress, std::uint32_t total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));

        hal::Screen::clear();
        rndr::Renderer::drawString(0, 0, "UP", rndr::font::DefaultFont);

        float percent = static_cast<float>(progress) / total;
        for (std::uint8_t i = 0; i < 16; i++) {
            if (i < percent * 16)
                hal::Screen::setPixel(i, 15, 0x20);
            else
                hal::Screen::setPixel(i, 15, 0x00);
        }

        hal::Screen::update();
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        switch (error) {
            case OTA_AUTH_ERROR:
                Serial.println("Auth Failed");
                break;
            case OTA_BEGIN_ERROR:
                Serial.println("Begin Failed");
                break;
            case OTA_CONNECT_ERROR:
                Serial.println("Connect Failed");
                break;
            case OTA_RECEIVE_ERROR:
                Serial.println("Receive Failed");
                break;
            case OTA_END_ERROR:
                Serial.println("End Failed");
                break;
            default:
                Serial.println("Unknown Error");
        }
    });
    ArduinoOTA.begin();
}

static void configureRTC() {
    configTime("CET-1CEST,M3.5.0/2,M10.5.0/3", "pool.ntp.org", "time.nist.gov");

    // Wait for time to be set
    tm timeInfo = {};
    time_t now = 0;
    while (timeInfo.tm_year + 1900 < 2000) {
        time(&now);
        localtime_r(&now, &timeInfo);
        delay(100);
        Serial.print(".");
    }

    Serial.println("\nRTC configured");
}

static void configureHardware() {
    Serial.begin(115200);

    hal::Button::init(2);
    hal::Screen::init(14, 12, 13, 15, 16);

    hal::Screen::clear(0x20);
    hal::Screen::update();
}

void setup() {
    configureHardware();
    configureWiFi();
    configureOTA();
    configureRTC();
}


void loop() {
    const static std::array<obegransad::Screen*, 6> Screens = {
        new obegransad::ScreenTime(),
        new obegransad::ScreenRain(),
        new obegransad::ScreenCircle(),
        new obegransad::ScreenGameOfLife(),
        new obegransad::ScreenFirework(),
        new obegransad::ScreenStars(),
    };


    static std::uint32_t currentScreenIndex = 0;
    static std::uint32_t lastSwitchTime = 0;
    static std::uint32_t lastUpdateTime = 0;

    {
        const auto currentTime = millis();
        auto currentScreen = Screens[currentScreenIndex];
        if (currentScreen == nullptr) {
            currentScreenIndex = 0;
            return;
        }

        static bool screenAppearing = true;
        if (screenAppearing) {
            screenAppearing = false;
            currentScreen->onShow();
            lastUpdateTime = 0;
            Serial.printf("Showing screen: %s\n", currentScreen->getName());
        }

        const auto updateInterval = currentScreen->getUpdateInterval();
        if (lastUpdateTime == 0 || updateInterval == 0 || currentTime > (lastUpdateTime + updateInterval)) {
            currentScreen->draw();
            lastUpdateTime = currentTime;
        }

        switch (hal::Button::getEvent()) {
            case hal::ButtonEvent::ShortPress:
                currentScreen->onButtonPress();
                lastUpdateTime = 0;
                break;
            case hal::ButtonEvent::LongPress:
                currentScreen->onButtonLongPress();
                lastUpdateTime = 0;
                break;
            case hal::ButtonEvent::Held:
                currentScreen->onButtonHeld();
                break;
            default:
                break;
        }

        if (currentTime > lastSwitchTime + currentScreen->getVisibleTime()) {
            auto nextScreenIndex = currentScreenIndex + 1;
            if (nextScreenIndex >= Screens.size()) {
                nextScreenIndex = 0;
            }

            lastSwitchTime = currentTime;

            if (currentScreenIndex != nextScreenIndex) {
                currentScreen->onHide();
                screenAppearing = true;
                currentScreenIndex = nextScreenIndex;
            }
        }
    }

    hal::Screen::update();
    hal::Button::update();
    ArduinoOTA.handle();
}