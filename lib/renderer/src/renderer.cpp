#include <hal/screen.hpp>
#include <renderer/renderer.hpp>
#include <renderer/fonts/font.hpp>

namespace rndr {

    void Renderer::drawBitmap(std::int8_t x, std::int8_t y, const Bitmap &bitmap, std::uint8_t brightness) {
        std::uint32_t bitIndex = 0;
        for (std::int8_t drawY = 0; drawY < bitmap.height; drawY++) {
            for (std::int8_t drawX = 0; drawX < bitmap.width; drawX++) {
                hal::Screen::setPixelNoOverwrite(
                    x + drawX,
                    y + drawY,
                    (bitmap.data[bitIndex / 8] & (1 << (7 - (bitIndex % 8)))) ? brightness : 0x00
                );
                bitIndex += 1;
            }
        }
    }

    void Renderer::drawString(std::int8_t x, std::int8_t y, const char* text, const std::array<Bitmap, 255> &font, std::uint8_t brightness) {
        while (*text != '\0') {
            const Bitmap &bitmap = font[static_cast<unsigned char>(*text)];
            drawBitmap(x, y, bitmap, brightness);
            x += bitmap.width + 1; // Move to the next character position
            text++;
        }
    }



}