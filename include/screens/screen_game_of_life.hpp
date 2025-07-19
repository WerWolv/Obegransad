#pragma once

#include <array>
#include <screens/screen.hpp>

namespace obegransad {

    class ScreenGameOfLife : public Screen {
    private:
        static constexpr auto NumGenerations = 200;
        static constexpr auto FrameCellCount = 3;
        static constexpr auto Width  = hal::Screen::Width + FrameCellCount * 2;
        static constexpr auto Height = hal::Screen::Height + FrameCellCount * 2;
        static constexpr auto TotalCells = Width * Height;

    public:
        const char* getName() const override {
            return "Game of Life";
        }

        std::uint32_t getUpdateInterval() const override { return 150; }

        void onShow() override {
            reset();
        }

        std::uint32_t getVisibleTime() const override { return 15 * 1000; }

        void draw() override {
            hal::Screen::clear();

            if (m_generations == 0) {
                reset();
            } else {
                nextGeneration();
                m_generations--;
            }

            for (std::uint8_t y = FrameCellCount; y < Height - FrameCellCount; y++) {
                for (std::uint8_t x = FrameCellCount; x < Width - FrameCellCount; x++) {
                    hal::Screen::setPixel(x - FrameCellCount, y - FrameCellCount, m_current[y * Width + x] ? 0x20 : 0x00);
                }
            }
        }

    private:
        void reset() {
            m_current.fill(0);
            m_previous.fill(0);

            for (auto& cell : m_current) {
                cell = random(3) == 0;
            }
            m_generations = NumGenerations;
        }

        constexpr void nextGeneration() {
            m_previous = m_current;
            for (std::uint8_t y = 0; y < Height; ++y) {
                for (std::uint8_t x = 0; x < Width; ++x) {
                    m_current[y * Width + x] = computeNextCellState(x, y);
                }
            }
        }

        constexpr std::uint8_t computeNextCellState(std::uint8_t x, std::uint8_t y) const {
            const auto liveNeighbours = countLiveNeighbours(x, y);
            const auto currentState = m_previous[y * Width + x];

            if (currentState == 1) {
                return (liveNeighbours == 2 || liveNeighbours == 3) ? 1 : 0;
            } else {
                return (liveNeighbours == 3) ? 1 : 0;
            }
        }

        constexpr std::uint8_t countLiveNeighbours(std::uint8_t x, std::uint8_t y) const {
            std::uint8_t count = 0;
            for (std::int8_t dy = -1; dy <= 1; ++dy) {
                for (std::int8_t dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0)
                        continue;

                    const auto nx = x + dx;
                    const auto ny = y + dy;

                    if (nx >= 0 && nx < Width && ny >= 0 && ny < Height) {
                        count += m_previous[ny * Width + nx];
                    }
                }
            }

            return count;
        }

    private:
        std::array<std::uint8_t, TotalCells> m_current = {};
        std::array<std::uint8_t, TotalCells> m_previous = {};
        std::uint32_t m_generations = 0;
    };

}
