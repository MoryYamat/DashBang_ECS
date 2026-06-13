#pragma once

#include <ddknd/ecs/ecs_fwd.h>
#include <ddknd/ecs/entity/entity.h>
#include <ddknd/math/math.h>


namespace app::camera
{
    struct CameraEntityDesc
    {
        ::ddknd::math::Vec3f position{0.0f, 0.0f, 5.0f};
        ::ddknd::math::Vec3f lookTarget{0.0f, 0.0f, 0.0f};
        ::ddknd::math::Vec3f up{0.0f, 1.0f, 0.0f};

        float fovYDeg = 60.0f;
        float nearZ = 0.1f;
        float farZ = 100.0f;

        bool mainCamera = true;
    };

    ::ddknd::ecs::Entity CreateCameraEntity(::ddknd::ecs::World& world, const CameraEntityDesc& desc);

    struct PlayerCameraRigSpawnDesc
    {
        ::ddknd::ecs::Entity target{};
        ::ddknd::ecs::Entity camera{};

        ::ddknd::math::Vec3f lookOffset{0.0f, 1.2f, 0.0f};// Camera's point of focus

        float yawDeg = -90.0f;
        float pitchDeg = 25.0f;
        float distance = 6.0f;
    };
    ::ddknd::ecs::Entity CreatePlayerCameraRig(::ddknd::ecs::World& world, const PlayerCameraRigSpawnDesc& desc);
} // namespace app::camera