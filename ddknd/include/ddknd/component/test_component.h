#pragma once

#include "ddknd/math/math.h"

#include <ddknd/component/gfx_component.h>

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

    struct DebugCameraControllerComponent
    {
        float yawDeg = -90.0f;
        float pitchDeg = 0.0f;

        float sensitivity = 0.1f;
        float moveSpeed = 5.0f;

        CameraLookComponent look{};
        CameraProjectionComponent projection{};
        CameraMatricesComponent matrices{};
    };
} // namespace ddknd::component