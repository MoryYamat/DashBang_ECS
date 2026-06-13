#pragma once

#include <ddknd/math/math.h>
#include <ddknd/ecs/ecs_fwd.h>

namespace app::camera
{
    struct CameraEntityDesc
    {
        ::ddknd::math::Vec3f position{0.0f, 0.0f, 5.0f};
        ::ddknd::math::Vec3f lookTarget{0.0f,0.0f,0.0f};
        ::ddknd::math::Vec3f up{0.0f, 1.0f, 0.0f};

        float fovYDeg = 60.0f;
        float nearZ = 0.1f;
        float farZ = 100.0f;

        bool mainCamera = true;
    };

    ::ddknd::ecs::Entity CreateCameraEntity(
        ::ddknd::ecs::World& world,
        const CameraEntityDesc& desc
    );
}