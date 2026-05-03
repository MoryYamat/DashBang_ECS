#pragma once

#include "math/math.h"

namespace ddknd::component
{
    struct Pos
    {
        float x;
        float y;
        float z;
    };

    struct Vel
    {
        float x;
        float y;
        float z;
    };

    struct Acc
    {
        float x;
        float y;
        float z;
    };


    struct CameraComponent
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