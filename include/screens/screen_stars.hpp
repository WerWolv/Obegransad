#pragma once

#include <array>
#include <screens/screen.hpp>

namespace obegransad {

    class ScreenStars : public Screen {
    public:
        const char *getName() const override {
            return "Stars";
        }

        void onShow() override {
            hal::Screen::clear();
            for (auto &star : m_stars) {
                star.x = random(hal::Screen::Width);
                star.y = random(hal::Screen::Height);
                star.brightness = random(255);
                star.lastUpdateTime = millis();
            }
        }

        std::uint32_t getUpdateInterval() const override { return 96; }
        std::uint32_t getVisibleTime() const override { return 10 * 1000; }

        void draw() override {
            for (std::int16_t fadeStep = 255; fadeStep >= 0; fadeStep -= 4) {
                for (auto &star : m_stars) {
                    if (star.brightness > 0) {
                        const auto currentTime = millis();
                        if (currentTime - star.lastUpdateTime >= 128) {
                            star.brightness = max(0, star.brightness - 8);
                            hal::Screen::setPixel(star.x, star.y, star.brightness);
                            star.lastUpdateTime = currentTime;
                        }
                    } else {
                        star.x = random(0, 16);
                        star.y = random(0, 16);
                        star.brightness = random(8, 255);

                        for (std::int16_t brightness = 0; brightness <= star.brightness; brightness += 5) {
                            const auto currentTime = millis();
                            if (currentTime - star.lastUpdateTime >= 64) {
                                hal::Screen::setPixel(star.x, star.y, brightness);
                                star.lastUpdateTime = currentTime;
                            }
                        }
                    }
                }
            }
        }

    private:
        struct Star {
            std::uint8_t x;
            std::uint8_t y;
            std::uint8_t brightness;
            std::uint32_t lastUpdateTime;
        };

        std::array<Star, 25> m_stars;
    };
}