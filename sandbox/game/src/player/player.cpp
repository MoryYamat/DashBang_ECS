#include "game/player/player.h"

#include <ddknd/ecs/ecs.h>
#include <ddknd/component/movement_component.h>
#include <ddknd/component/gfx_component.h>

#include "game/component/character_stats_component.h"
#include "game/component/IntentComponent.h"
#include "game/assets/actor/paladin_assets.h"

namespace app::player
{
    ::ddknd::ecs::Entity CreatePaladinPlayer(::ddknd::ecs::World& world, const app::assets::actor::PaladinAssetIDs& paladinAssets, const PlayerSpawnDesc& desc)
    {
        auto e = CreatePaladin(world, paladinAssets);

        auto& reg = world.GetRegistry();
        auto& transform = reg.GetComponent<::ddknd::component::TransformComponent>(e);
        transform.localTRS.translation = desc.position;
        transform.dirty = true;

        reg.AddComponent<app::component::PlayerControlComponent>(e);
        reg.AddComponent<app::component::MovementIntentComponent>(e);
        reg.AddComponent<app::component::CharacterMoveStatsComponent>(e, app::component::CharacterMoveStatsComponent{.moveSpeed = desc.moveSpeed});

        reg.AddComponent<::ddknd::component::VelocityComponent>(e);

        return e;
    }
}