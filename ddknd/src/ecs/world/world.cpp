#include "ddknd/ecs/world/world.h"

#include "ddknd/ecs/entity/entity.h"
#include "ddknd/ecs/registry/registry.h"

namespace ddknd::ecs
{
    using Registry = ::ddknd::ecs::Registry;
    using Entity = ::ddknd::ecs::Entity;
    struct World::Impl
    {
        Registry regs;
    };

    World::World() : impl_(std::make_unique<Impl>()) {}

    World::~World() {}

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
} // namespace ddknd::ecs