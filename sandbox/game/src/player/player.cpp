#include "game/player/player.h"

#include <ddknd/component/gfx_component.h>
#include <ddknd/component/movement_component.h>
#include <ddknd/ecs/ecs.h>


#include "game/assets/actor/paladin_assets.h"
#include "game/component/IntentComponent.h"
#include "game/component/character_stats_component.h"
#include <ddknd/component/hitbox_component.h>


#include "game/component/state_component.h"

namespace app::player
{
    ::ddknd::ecs::Entity CreatePaladinPlayer(::ddknd::ecs::World& world,
                                             const app::assets::actor::PaladinAssetIDs& paladinAssets,
                                             const PlayerSpawnDesc& desc)
    {
        auto e = CreatePaladin(world, paladinAssets);

        auto& reg = world.GetRegistry();
        auto& transform = reg.GetComponent<::ddknd::component::TransformComponent>(e);
        transform.localTRS.translation = desc.position;
        transform.dirty = true;

        // ============== actor intent / resoloved intent ==============
        reg.AddComponent<app::component::MovementIntentComponent>(e);
        reg.AddComponent<app::component::AttackIntentComponent>(e);

        // ============== movement logic ==============
        reg.AddComponent<app::component::CharacterMoveStatsComponent>(
            e, app::component::CharacterMoveStatsComponent{.moveSpeed = desc.moveSpeed});
        reg.AddComponent<::ddknd::component::VelocityComponent>(e);

        // =============== Action ===============
        // ******* attack logic  *******
        reg.AddComponent<app::component::AttackStateComponent>(e);
        
        app::component::AttackHitboxDef attackDef{.radius = 1.2f, .lifetime = 0.32f, .localOffset = ddknd::math::Vec3f{0.0f, 0.75f, 0.3f}, .useforwardHemisphere = true};

        // transition definition
        reg.AddComponent<app::component::AttackDefComponent>(e, app::component::AttackDefComponent{.hitbox = attackDef});
        reg.AddComponent<app::component::CharacterControlModifierComponent>(e);

        reg.AddComponent<app::component::PlayerLocomotionStateComponent>(e);

        reg.AddComponent<ddknd::component::HurtboxComponent>(e);
        //@TODO: move actor-specific hurtbox settings to ActorDef / data asset.
        reg.AddComponent<ddknd::component::SphereHurtboxComponent>(e, ddknd::component::SphereHurtboxComponent{.localOffset = {0.0f, 0.75f, 0.0f}});

        
        return e;
    }
} // namespace app::player