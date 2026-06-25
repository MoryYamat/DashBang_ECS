#include "game/actor/character/npc.h"

#include <ddknd/ecs/ecs.h>
#include <ddknd/component/gfx_component.h>

#include "game/assets/actor/paladin_assets.h"
#include "game/component/state_component.h"

#include <ddknd/component/hitbox_component.h>

namespace app::actor
{
    ddknd::ecs::Entity CreatePaladinNPC(ddknd::ecs::World& world, const app::assets::actor::PaladinAssetIDs& paladinAssets,
                          const NPCSpawnDesc& desc)
    {
        auto e = CreatePaladin(world, paladinAssets);

        auto& reg = world.GetRegistry();
        auto& transform = reg.GetComponent<ddknd::component::TransformComponent>(e);
        transform.localTRS.translation = desc.position;
        transform.dirty = true;

        reg.AddComponent<ddknd::component::HurtboxComponent>(e);
        reg.AddComponent<ddknd::component::SphereHurtboxComponent>(e, ddknd::component::SphereHurtboxComponent{.localOffset = {0.0f, 0.75f, 0.0f}});
        reg.AddComponent<app::component::PlayerLocomotionStateComponent>(e);
        reg.AddComponent<app::component::AttackStateComponent>(e);

        return e;
    }
} // namespace app::actor