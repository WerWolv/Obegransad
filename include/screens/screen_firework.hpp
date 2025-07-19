#pragma once

#include <cmath>
#include <screens/screen.hpp>

namespace obegransad {

    class ScreenFirework : public Screen {
    public:
        const char *getName() const override {
            return "Firework";
        }

        void onShow() override {
            hal::Screen::clear();
        }

        std::uint32_t getUpdateInterval() const override { return 1; }
        std::uint32_t getVisibleTime() const override { return 5 * 1000; }

        void draw() override {
            unsigned long currentMillis = millis();

            switch (m_state) {
                case State::Rocket:
                    if (currentMillis - m_previousMillis >= RocketDelay) {
                        m_previousMillis = currentMillis;
                        hal::Screen::clear();
                        hal::Screen::setPixel(m_rocketX, m_rocketY, 255);
                        m_rocketY--;

                        if (m_rocketY < random(8)) {
                            m_state = State::Explosion;
                            m_explosionStartMillis = currentMillis;
                            m_explosionRadius = 1;
                            m_maxRadius = random(3, 6);
                            m_explosionX = m_rocketX;
                            m_explosionY = m_rocketY;
                        }
                    }
                    break;

                case State::Explosion:
                    if (currentMillis - m_explosionStartMillis >= ExplosionDelay) {
                        m_explosionStartMillis = currentMillis;
                        drawExplosion(m_explosionX, m_explosionY, m_explosionRadius, 255);
                        m_explosionRadius++;
                        if (m_explosionRadius > m_maxRadius) {
                            m_state = State::Fade;
                            m_fadeStartMillis = currentMillis;
                            m_brightness = 248;
                        }
                    }
                    break;

                case State::Fade:
                    if (currentMillis - m_fadeStartMillis >= FadeDelay) {
                        m_fadeStartMillis = currentMillis;
                        drawExplosion(m_explosionX, m_explosionY, m_maxRadius, m_brightness);
                        m_brightness -= 8;
                        if (m_brightness < 0) {
                            m_state = State::Wait;
                            m_waitStartMillis = currentMillis;
                        }
                    }
                    break;

                case State::Wait:
                    if (currentMillis - m_waitStartMillis >= ExplosionDuration) {
                        m_rocketY = 16;
                        m_rocketX = random(16);
                        m_state = State::Rocket;
                        m_previousMillis = currentMillis;
                    }
                    break;
            }
        }

    private:
        enum class State { Rocket, Explosion, Fade, Wait };
        State m_state = State::Rocket;

        int m_rocketY = 16;
        int m_rocketX = 0;

        std::uint8_t m_explosionX = 0;
        std::uint8_t m_explosionY = 0;
        std::uint8_t m_explosionRadius = 1;
        std::uint8_t m_maxRadius = 3;
        std::int16_t m_brightness = 255;

        std::uint32_t m_previousMillis = 0;
        std::uint32_t m_explosionStartMillis = 0;
        std::uint32_t m_fadeStartMillis = 0;
        std::uint32_t m_waitStartMillis = 0;

        void drawExplosion(std::uint8_t x, std::uint8_t y, std::uint8_t maxRadius, std::uint8_t brightness) {
            for (std::uint8_t i = 0; i < hal::Screen::Width; i++) {
                for (std::uint8_t j = 0; j < hal::Screen::Height; j++) {
                    const auto distance = std::round(std::sqrt(std::pow(i - x, 2) + std::pow(j - y, 2)));
                    if (distance <= maxRadius) {
                        hal::Screen::setPixel(i, j, brightness);
                    }
                }
            }
        }

        constexpr static auto ExplosionDelay = 60;
        constexpr static auto FadeDelay = 24;
        constexpr static auto RocketDelay = 60;
        constexpr static auto ExplosionDuration = 500;
    };
}