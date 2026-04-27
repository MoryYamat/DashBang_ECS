#include "ecs/world/world.h"

#include "ecs/entity/entity.h"
#include "ecs/registry/registry.h"

namespace ddknd::world
{
    using Registry = ::ddknd::registry::Registry;
    using Entity = ::ddknd::entity::Entity;
    struct World::Impl
    {
        Registry regs;
    };

    World::World()
        : impl_(std::make_unique<Impl>())
    {
        
    }

    World::~World()
    {

    }

    Entity World::Create()
    {
        return impl_->regs.Create();
    }

    void World::Destroy(Entity e)
    {
        impl_->regs.Destroy(e);
    }

    bool World::IsAlive(Entity e) const
    {
        return impl_->regs.IsAlive(e);
    }

    Registry& World::GetRegistry()
    {
        return impl_->regs;
    }

    const Registry& World::GetRegistry() const
    {
        return impl_->regs;
    }
}// namespace ddknd::world