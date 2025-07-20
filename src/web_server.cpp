#include <web_server.hpp>
#include <ESP8266WebServer.h>
#include <hal/screen.hpp>

namespace obegransad::web {

    static ESP8266WebServer *s_server;
    static std::uint32_t s_notificationTime = 0;

    static void configureRoutes() {
        s_server->on("/notification", HTTP_POST, [] {
            Serial.println("notification");
            s_notificationTime = millis() + 2000;
            s_server->send(200, "text/html", "Triggered notification");
        });
    }

    void configure() {
        s_server = new ESP8266WebServer(80);
        s_server->begin();

        configureRoutes();
    }

    void process() {
        if (s_server == nullptr)
            return;

        s_server->handleClient();

        const auto currTime = millis();
        if (s_notificationTime > 0 && currTime < s_notificationTime) {
            if (currTime % 500 < 250) {
                hal::Screen::clear(0x00);
            } else {
                hal::Screen::clear(0xFF);
            }
        }
    }

}
