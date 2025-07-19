#pragma once

#include <array>
#include <cstdint>

namespace hal {

    class Screen {
    public:
        constexpr static auto Width    = 16;
        constexpr static auto Height = 16;
        constexpr static auto BrightnessLevels = 64;

        static void init(std::uint8_t sck, std::uint8_t miso, std::uint8_t mosi, std::uint8_t cs, std::uint8_t en);
        static void update();

        static void setPixel(std::int8_t x, std::int8_t y, std::uint8_t brightness = 0xFF);
        static void setPixelNoOverwrite(std::int8_t x, std::int8_t y, std::uint8_t brightness = 0xFF);
        static std::uint8_t getPixel(std::int8_t x, std::int8_t y);
        static void setAllPixels(const std::array<std::uint8_t, Width * Height> &pixels);
        static void clear(std::uint8_t brightness = 0x00);

        static bool isTurnedOn();
        static void turnOn();
        static void turnOff();

    private:
        Screen() = default;
    };

}