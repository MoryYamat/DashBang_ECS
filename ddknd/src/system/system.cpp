#include <ddknd/system/system.h>

#include <ddknd/ecs/query/query.h>
#include <ddknd/ecs/world/world.h>

namespace ddknd::system
{
    void EngineSystemRunner::Update(::ddknd::world::World& world, const ::ddknd::system::SystemContext& ctx)
    {
        RunTransform(world, ctx);
    }

    void EngineSystemRunner::RunTransform(::ddknd::world::World& world, const ::ddknd::system::SystemContext& ctx)
    {
        // using namespace ecs;
        // auto q_transform = query.
        // world.GetRegistry()
    }
}