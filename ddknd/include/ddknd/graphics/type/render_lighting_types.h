#pragma once

#include "ddknd/math/math.h"

namespace ddknd::graphics::types
{
    // simple light
    struct DirectionalLight
    {
        math::Vec3f directionWolrd{-0.3f, -1.0f, -0.4f};
        math::Vec3f color {1.0f, 1.0f,1.0f};
        float intensity = 1.0f;
    };
    struct RenderLighting
    {
        DirectionalLight mainLight{};
        float ambientStrength = 0.45f;
    };
}