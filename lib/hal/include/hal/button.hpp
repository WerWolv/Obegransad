#pragma once

#include <cstdint>

namespace hal {

    enum class ButtonEvent {
        None,
        ShortPress,
        LongPress,
        Held
    };

    class Button {
    public:
        static void init(std::uint8_t pin);
        static void update();
        static ButtonEvent getEvent();
        static bool isHeld();

    private:
        Button() = default;
    };

}