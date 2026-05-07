#pragma once

#include "math/math.h"

namespace ddknd::component
{
    struct Pos
    {
        using Vec3f = ::ddknd::math::Vec3f;
        Vec3f v{};
    };

    struct Vel
    {
        using Vec3f = ::ddknd::math::Vec3f;
        Vec3f v{};
    };

    struct Acc
    {
        using Vec3f = ::ddknd::math::Vec3f;
        Vec3f v{};
    };


    struct DebugCameraComponent
    {
        math::Vec3f pos{0.0f, 0.0f, 0.0f};
        math::Vec3f target;
        math::Vec3f up{0.0f, 1.0f, 0.0f};

        float fovRadians;
        float aspect;
        float nearZ = 0.1f;
        float farZ = 1000.0f;
    };
}// namespace ddknd::component