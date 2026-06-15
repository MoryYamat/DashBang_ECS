#include "game/system/logic/game_character_facing_system.h"

#include "game/component/IntentComponent.h"

#include <ddknd/component/gfx_component.h>
#include <ddknd/math/math.h>

namespace app::system
{
    void CharacterFacingSystem::UpdateOne(::ddknd::component::TransformComponent& transform,
                                              const app::component::MovementIntentComponent& moveIntent)
    {
        if(!moveIntent.active)
            return;

        auto dir = moveIntent.direction;
        dir.y() = 0.0f;

        if(::ddknd::math::lengthSquared(dir) <= ::ddknd::math::kEpsilonSq<float>)
        {
            return;
        }

        const float yaw = std::atan2(dir.x(), dir.z());

        transform.localTRS.rotation = ::ddknd::math::FromAxisAngle(::ddknd::math::Vec3f{0.0f,1.0f, 0.0f}, yaw);

        transform.dirty = true;
    }
} // namespace app::system