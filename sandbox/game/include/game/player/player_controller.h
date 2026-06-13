#pragma once

#include <ddknd/ecs/entity/entity.h>
#include <ddknd/ecs/ecs_fwd.h>

namespace app::player
{
    struct PlayerControllerSpawnDesc
    {
        ::ddknd::ecs::Entity actor{};
        ::ddknd::ecs::Entity cameraRig{};
    };

    ::ddknd::ecs::Entity CreateLocalPlayerController(::ddknd::ecs::World& world, const PlayerControllerSpawnDesc& desc);
}