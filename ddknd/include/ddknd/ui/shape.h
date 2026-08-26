#pragma once

#include <ddknd/math/math.h>

namespace ddknd::ui
{
    struct UIShape
    {
        ddknd::math::Vec2f position{};  // Screen position
        ddknd::math::Vec4f color{};

        float width = 0;
        float height = 0;
    };

    struct UIRectButton
    {
        UIShape shape;

        bool pressed = false;
    };
}