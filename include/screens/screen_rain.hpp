#pragma once

#include <array>
#include <screens/screen.hpp>

namespace obegransad {

    class ScreenRain : public Screen {
    public:
        const char *getName() const override {
            return "Rain";
        }

        void onShow() override {
            for (auto &drop : m_drops) {
                drop.x = 0;
                drop.y = 0;
                drop.visible = false;
            }
        }

        std::uint32_t getUpdateInterval() const override { return 100; }
        std::uint32_t getVisibleTime() const override { return 5 * 1000; }

        void draw() override {
            // dim the trail
            for (uint8_t x = 0; x < hal::Screen::Width; x++) {
                for (uint8_t y = 0; y < hal::Screen::Height; y++)
                {
                    uint8_t brightness = hal::Screen::getPixel(x, y);

                    if (brightness <= 25)
                        brightness = 0;
                    else if (brightness <= 50)
                        brightness = 16;
                    else if (brightness <= 75)
                        brightness = 32;
                    else
                        brightness = 64;

                    hal::Screen::setPixel(x, y, brightness);
                }
            }

            // draw the drops
            for (auto &drop : m_drops) {
                if (!drop.visible)
                {
                    if (random(10) > 1)
                        continue;

                    drop.x = random(hal::Screen::Width);
                    drop.y = 0;
                    drop.visible = true;

                    hal::Screen::setPixel(drop.x, drop.y, 255);
                } else {
                    drop.y++;
                    if (drop.y >= hal::Screen::Height) {
                        drop.visible = false;
                        continue;
                    }
                    hal::Screen::setPixel(drop.x, drop.y, 255);
                }
            }
        }

    private:
        struct Drop {
            unsigned char x;
            unsigned char y;
            bool visible;
        };

        std::array<Drop, 15> m_drops = {};
    };
}