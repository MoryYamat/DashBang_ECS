#pragma once

#include "game/assets/actor/assets_actor_fwd.h"

#include <ddknd/ecs/ecs_fwd.h>

#include <ddknd/math/math.h>

namespace app::player
{
    struct PlayerSpawnDesc
    {
        ::ddknd::math::Vec3f position{0.0f, 0.0f, 0.0f};

        float moveSpeed = 0.5f;
    };

    ::ddknd::ecs::Entity CreatePaladinPlayer(::ddknd::ecs::World& world, const app::assets::actor::PaladinAssetIDs& paladinAssets, const PlayerSpawnDesc& desc = {});
}