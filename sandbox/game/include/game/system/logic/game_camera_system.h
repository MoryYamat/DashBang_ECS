#pragma once
#include "game/component/game_component_fwd.h"
#include <ddknd/component/component_fwd.h>

namespace app::system
{
    struct CameraDesiredPoseSystem
    {
        static void UpdateOne(::app::component::CameraDesiredPoseComponent& desired,
                              const ::app::component::CameraFollowTargetComponent& follow,
                              const ::app::component::CameraOrbitComponent& orbit,
                              const ::ddknd::component::TransformComponent& targetTransform);
    };

    struct CameraApplySystem
    {
        static void UpdateOne(::ddknd::component::TransformComponent& cameraTransform,
                              ::ddknd::component::CameraLookComponent& cameraLook,
                              const app::component::CameraDesiredPoseComponent& desired);
    };
} // namespace app::system