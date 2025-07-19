#include <Arduino.h>
#include <hal/button.hpp>

namespace hal {

    static std::uint8_t s_buttonPin = 0;
    static std::int32_t s_buttonDownSince = -1;
    static ButtonEvent s_lastEvent = ButtonEvent::None;

    void Button::init(std::uint8_t pin) {
        ::pinMode(pin,  INPUT);
        s_buttonPin = pin;
    }

    void Button::update() {
        if (s_lastEvent == ButtonEvent::ShortPress || s_lastEvent == ButtonEvent::LongPress)
            return;

        if (isHeld()) {
            if (s_buttonDownSince == -1)
                s_buttonDownSince = millis();

            s_lastEvent = ButtonEvent::Held;
        } else {
            const auto pressTime = millis() - s_buttonDownSince;
            if (s_buttonDownSince == -1) {
                s_lastEvent = ButtonEvent::None;
            } else if (pressTime < 800) {
                s_lastEvent = ButtonEvent::ShortPress;
            } else {
                s_lastEvent = ButtonEvent::LongPress;
            }

            s_buttonDownSince = -1;
        }
    }

    ButtonEvent Button::getEvent() {
        auto result = s_lastEvent;
        s_lastEvent = ButtonEvent::None;

        return result;
    }

    bool Button::isHeld() {
        return ::digitalRead(s_buttonPin) == LOW;
    }


}