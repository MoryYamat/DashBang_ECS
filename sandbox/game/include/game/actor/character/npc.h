#pragma once

#include <ddknd/ecs/ecs_fwd.h>

#include "game/assets/actor/assets_actor_fwd.h"

#include <ddknd/math/math.h>

namespace app::actor
{
    struct NPCSpawnDesc
    {
        ddknd::math::Vec3f position{};
    };

    ddknd::ecs::Entity CreatePaladinNPC(ddknd::ecs::World& world, const app::assets::actor::PaladinAssetIDs& paladinAssets,
                          const NPCSpawnDesc& desc);

    ddknd::ecs::Entity CreateMutantNPC(ddknd::ecs::World& world, const app::assets::actor::MutantAssetIDs& mutantAssets,
                          const NPCSpawnDesc& desc);
} // namespace app::actor