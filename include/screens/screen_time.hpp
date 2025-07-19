#pragma once

#include <Arduino.h>
#include <ctime>
#include <screens/screen.hpp>

namespace obegransad {

    class ScreenTime : public Screen {
    public:
        const char *getName() const override {
            return "Time";
        }

        std::uint32_t getUpdateInterval() const override { return 10000; }
        std::uint32_t getVisibleTime() const override { return 5 * 60 * 1000; }

        void onShow() override {
            hal::Screen::clear();

            updateTime();
        }

        void onButtonPress() override {
            if (hal::Screen::isTurnedOn()) {
                hal::Screen::turnOff();
            } else {
                hal::Screen::turnOn();
            }
        }

        void onButtonLongPress() override {
            ESP.restart();
        }

        void draw() override {
            updateTime();

            hal::Screen::clear();

            rndr::Renderer::drawBitmap(-1, 0, rndr::font::BigNumbers[m_timeInfo.tm_hour / 10], 0x20);
            rndr::Renderer::drawBitmap(8, 0, rndr::font::BigNumbers[m_timeInfo.tm_hour  % 10], 0x20);

            rndr::Renderer::drawBitmap(-1, 8, rndr::font::BigNumbers[m_timeInfo.tm_min / 10], 0x20);
            rndr::Renderer::drawBitmap(8, 8, rndr::font::BigNumbers[m_timeInfo.tm_min  % 10], 0x20);
        }

    private:
        void updateTime() {
            time_t now = 0;
            time(&now);
            localtime_r(&now, &m_timeInfo);
        }

    private:
        tm m_timeInfo = {};
    };

}