#pragma once

#include <cstdint>
#include <hal/screen.hpp>
#include <renderer/renderer.hpp>
#include <renderer/fonts/font.hpp>

namespace  {

    class Screen {
    public:
        virtual ~Screen() = default;

        virtual const char *getName() const = 0;
        virtual void onShow() {}
        virtual void onHide() {}

        virtual std::uint32_t getVisibleTime() const { return 30 * 1000;}
        virtual std::uint32_t getUpdateInterval() const { return 0; }

        virtual void draw() = 0;
        virtual void onButtonPress() {}
        virtual void onButtonLongPress() {}
        virtual void onButtonHeld() {}
    };

}