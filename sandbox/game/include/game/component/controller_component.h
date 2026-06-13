#pragma once

#include <ddknd/ecs/entity/entity.h>
#include <ddknd/math/math.h>

namespace app::component
{
    struct PlayerControllerComponent
    {
        // player-controlled entity marker
    };
    struct ControlledActorComponent
    {
        ::ddknd::ecs::Entity actor{};
    };
    struct ControlledCameraRigComponent
    {
        ::ddknd::ecs::Entity cameraRig{};
    };
    struct PlayerCameraRigComponent
    {
        bool enabled = true;
    };
    struct CameraFollowTargetComponent
    {
        ::ddknd::ecs::Entity target{};

        // From the player's starting point, how much should the target of their gaze be shifted?
        ::ddknd::math::Vec3f lookOffset{0.0f, 1.2f, 0.0f};// Camera's point of focus
    };

    // 
    struct CameraOrbitComponent
    {
        float yawDeg = -90.0f;
        float pitchDeg = 25.0f;
        float distance = 6.0f;

        float minPitchDeg = -80.0f;
        float maxPitchDeg = 80.0f;

        float minDistance = 2.0f;
        float maxDistance = 12.0f;
    };

    // requested camera pose component(pos, looktarget)
    struct CameraDesiredPoseComponent
    {
        bool valid = false;

        ::ddknd::math::Vec3f position{0.0f, 0.0f, 0.0f};
        ::ddknd::math::Vec3f lookTarget{0.0f, 0.0f, 0.0f};
    };

    // This indicates which cameraEntity this CameraRing will manipulate.
    struct CameraOutputComponent
    {
        ::ddknd::ecs::Entity camera{};
    };
} // namespace app::component