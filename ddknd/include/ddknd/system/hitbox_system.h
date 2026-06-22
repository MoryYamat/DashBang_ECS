#pragma once

#include <ddknd/ecs/entity/entity.h>
#include <ddknd/ecs/ecs_fwd.h>

#include <ddknd/system/system_fwd.h>


namespace ddknd::system
{
    struct HitboxCollisionSystem
    {
        static void Update(ddknd::ecs::World& world, const ddknd::system::FrameContext& ctx);
    };
}