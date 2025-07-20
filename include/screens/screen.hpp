#pragma once

#include <cstdint>
#include <hal/screen.hpp>
#include <renderer/renderer.hpp>
#include <renderer/fonts/font.hpp>

namespace obegransad {

    class Screen {
    public:
        virtual ~Screen() = default;

        [[nodiscard]] virtual const char *getName() const = 0;
        virtual void onShow() {}
        virtual void onHide() {}

        [[nodiscard]] virtual std::uint32_t getVisibleTime() const { return 30 * 1000;}
        [[nodiscard]] virtual std::uint32_t getUpdateInterval() const { return 0; }

        virtual void draw() = 0;
        virtual void onButtonPress() {}
        virtual void onButtonLongPress() {}
        virtual void onButtonHeld() {}
    };

}