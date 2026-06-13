#include "game/system/logic/game_camera_system.h"

#include <ddknd/component/gfx_component.h>
#include <ddknd/math/math.h>
#include "game/component/controller_component.h"

#include <cmath>

namespace app::system
{
    void CameraDesiredPoseSystem::UpdateOne(::app::component::CameraDesiredPoseComponent& desired,
                                            const ::app::component::CameraFollowTargetComponent& follow,
                                            const ::app::component::CameraOrbitComponent& orbit,
                                            const ::ddknd::component::TransformComponent& targetTransform)
    {
        const auto& targetPos = targetTransform.localTRS.translation;
        const auto& lookTarget = targetPos + follow.lookOffset; // position vector

        const float yaw = ::ddknd::math::degToRadf(orbit.yawDeg);
        const float pitch = ::ddknd::math::degToRadf(orbit.pitchDeg);

        const float cp = std::cos(pitch);
        const float sp = std::sin(pitch);
        const float cy = std::cos(yaw);
        const float sy = std::sin(yaw);

        // target -> camera offset
        ::ddknd::math::Vec3f offset{cp * cy * orbit.distance, sp * orbit.distance, cp * sy * orbit.distance};

        desired.lookTarget = lookTarget;
        desired.position = lookTarget + offset;
        desired.valid = true;
    }

    void CameraApplySystem::UpdateOne(::ddknd::component::TransformComponent& cameraTransform,
                                      ::ddknd::component::CameraLookComponent& cameraLook,
                                      const app::component::CameraDesiredPoseComponent& desired)
    {
        if (!desired.valid)
        {
            return;
        }

        cameraTransform.localTRS.translation = desired.position;
        cameraTransform.dirty = true;

        const auto toTarget = desired.lookTarget - desired.position; // direction vector

        if (::ddknd::math::lengthSquared(toTarget) > ::ddknd::math::kEpsilonSq<float>)
        {
            cameraLook.forward = ::ddknd::math::normalize(toTarget);
        }
    }
} // namespace app::system