#include <ddknd/system/lifetime_system.h>

#include <ddknd/ecs/ecs.h>
#include <ddknd/component/general_component.h>

namespace ddknd::system
{
    
    void LifetimeSystem::Update(ddknd::ecs::World& world, const float dt)
    {
        using namespace ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view = reg.view(query().select<ddknd::component::LifetimeComponent>()).withEntity();

        std::vector<Entity> dead;

        for (auto [e, lifetime] : view)
        {
            lifetime.elapsed += dt;

            if (lifetime.elapsed >= lifetime.lifetime)
            {
                dead.push_back(e);
            }
        }

        for (auto e : dead)
        {
            // for debug
            // ddknd::ecs::Entity old = e;
            // std::cerr << "hitbox " << e.Value() << " destroyed\n";

            reg.Destroy(e);

            // for debug
            // assert(!reg.IsAlive(old));
            // assert(reg.TryGetComponent<app::component::HitboxComponent>(old) == nullptr);
        }
    }
}