#pragma once

#include <cstdint>
#include <vector>
#include <span>

namespace rndr {

    struct Bitmap {
        std::uint8_t width;
        std::uint8_t height;
        std::vector<std::uint8_t> data;
    };

    class Renderer {
    public:
        static void drawBitmap(std::int8_t x, std::int8_t y, const Bitmap &bitmap, std::uint8_t brightness = 0xFF);
        static void drawString(std::int8_t x, std::int8_t y, const char *text, const std::array<Bitmap, 255> &font, std::uint8_t brightness = 0xFF);

    private:
        Renderer() = default;
    };

}
