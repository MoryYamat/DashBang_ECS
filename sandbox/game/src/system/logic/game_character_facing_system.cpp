#include "game/system/logic/game_character_facing_system.h"

#include "game/component/IntentComponent.h"
#include "game/component/character_stats_component.h"

#include <ddknd/component/gfx_component.h>
#include <ddknd/math/math.h>

namespace app::system
{
    void CharacterFacingSystem::UpdateOne(::ddknd::component::TransformComponent& transform,
                                              const app::component::MovementIntentComponent& moveIntent,
                                              const app::component::CharacterControlModifierComponent& modifier)
    {
        if(!moveIntent.active)
            return;

        if(modifier.lockFacing)
        {
            return;
        }

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