#include "game/system/resolve/game_movement_intent_resolve_system.h"

#include "game/component/IntentComponent.h"
#include "game/component/controller_component.h"
#include <ddknd/component/gfx_component.h>

#include <algorithm>
#include <ddknd/math/math.h>


namespace app::system
{
    void MovementIntentResolveSystem::UpdateOne(app::component::MovementIntentComponent& out,
                                                const app::component::RequestedMovementIntentComponent& request,
                                                const ddknd::component::CameraLookComponent& cameraLook)
    {
        if (!request.active)
        {
            out.active = false;
            out.direction = {0.0f, 0.0f, 0.0f};
            return;
        }

        auto forward = cameraLook.forward;

        // xz-plane
        forward.y() = 0.0f;

        if (::ddknd::math::lengthSquared(forward) <= ::ddknd::math::kEpsilonSq<float>)
        {
            out.active = false;
            out.direction = {0.0f, 0.0f, 0.0f};
            return;
        }

        forward = ::ddknd::math::normalize(forward);

        // right-handed
        ::ddknd::math::Vec3f right{-forward.z(), 0.0f, forward.x()};

        auto move = right * request.moveAxis.x() + forward * request.moveAxis.y();

        if (::ddknd::math::lengthSquared(move) > ::ddknd::math::kEpsilonSq<float>)
        {
            out.direction = ::ddknd::math::normalize(move);
            out.active = true;
        }
        else
        {
            out.direction = {0.0f, 0.0f, 0.0f};
            out.active = false;
        }
    }

    void CameraIntentResolveSystem::UpdateOne(app::component::CameraOrbitComponent& orbit,
                                              const app::component::RequestedCameraIntentComponent& request)
    {
        if (!request.active)
            return;

        orbit.yawDeg += request.yawDeltaDeg;
        orbit.pitchDeg += request.pitchDeltaDeg;

        orbit.pitchDeg = std::clamp(orbit.pitchDeg, orbit.minPitchDeg, orbit.maxPitchDeg);
        orbit.distance = std::clamp(orbit.distance + request.zoomDelta, orbit.minDistance, orbit.maxDistance);
    }

    void AttackIntentResolveSystem::UpdateOne(app::component::AttackIntentComponent& out,
                                              const app::component::RequestedAttackIntentComponent& request)
    {
        out.active = false;
        
        if(!request.active)
        {
            return;
        }
            
        out.active = true;
    }
} // namespace app::system