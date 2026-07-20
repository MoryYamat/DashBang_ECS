#pragma once

#include <ddknd/ecs/entity/entity.h>
#include <ddknd/ecs/ecs_fwd.h>
#include "game/assets/item/assets_item_fwd.h"

#include <ddknd/math/math.h>

namespace app::actor
{
    struct ItemSpawnDesc
    {
        ddknd::math::Vec3f position{};
        ddknd::math::Vec3f scale{ddknd::math::Vec3f::One()};
    };


    ddknd::ecs::Entity CreateAntiqueCameraItem(ddknd::ecs::World& world, const app::assets::item::AntiqueCameraAssetsIDs& antiqueCameraAssets,
                          const ItemSpawnDesc& desc);
}