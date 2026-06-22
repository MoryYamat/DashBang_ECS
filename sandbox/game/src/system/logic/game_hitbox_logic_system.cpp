#include "game/system/logic/game_hitbox_logic_system.h"

#include "game/component/character_stats_component.h"
#include "game/component/hitbox_component.h"
#include "game/component/state_component.h"
#include <ddknd/component/gfx_component.h>
#include <ddknd/ecs/ecs.h>

namespace app::system
{
    void AttackHitboxSpawnSystem::Update(ddknd::ecs::World& world)
    {
        using namespace ::ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view = reg.view(query()
                                 .select<app::component::AttackStateComponent>()
                                 .require<app::component::AttackDefComponent, ddknd::component::TransformComponent>())
                        .withEntity();

        std::vector<ddknd::ecs::Entity> owners;

        for (auto [owner, state, def, transform] : view)
        {
            if (state.current != app::component::AttackState::Active)
            {
                continue;
            }

            if (state.hitboxSpawned)
            {
                continue;
            }

            owners.push_back(owner);
        }

        for (auto owner : owners)
        {

            auto* state = reg.TryGetComponent<app::component::AttackStateComponent>(owner);
            auto* def = reg.TryGetComponent<app::component::AttackDefComponent>(owner);
            auto* transform = reg.TryGetComponent<ddknd::component::TransformComponent>(owner);

            if (!state || !def || !transform)
                continue;
            if (state->hitboxSpawned)
                continue;

            auto hitbox = reg.Create();
            std::cerr << "hibox entity " << hitbox.Value() << " created\n";

            ddknd::component::TransformComponent hitboxTransform{};
            hitboxTransform.localTRS.translation =
                ddknd::math::TransformPoint(transform->worldMatrix, def->hitbox.localOffset);
            hitboxTransform.worldMatrix = hitboxTransform.localTRS.ToMatrix();
            hitboxTransform.dirty = false;
            reg.AddComponent<ddknd::component::TransformComponent>(hitbox, hitboxTransform);

            // hitbox
            reg.AddComponent<app::component::HitboxComponent>(
                hitbox,
                app::component::HitboxComponent{.owner = owner, .lifetime = def->hitbox.lifetime, .elapsed = 0.0f});

            reg.AddComponent<app::component::HemisphereHitboxComponent>(
                hitbox, app::component::HemisphereHitboxComponent{
                            .forward = ddknd::math::TransformDirection(transform->worldMatrix,
                                                                       ddknd::math::Vec3f{0.0f, 0.0f, 1.0f}),
                            .radius = def->hitbox.radius});
            state->hitboxSpawned = true;
        }
    }

    void HitboxLifetimeSystem::Update(ddknd::ecs::World& world, const float dt)
    {
        using namespace ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view = reg.view(query().select<app::component::HitboxComponent>()).withEntity();

        std::vector<Entity> dead;

        for (auto [e, hitbox] : view)
        {
            hitbox.elapsed += dt;

            if (hitbox.elapsed >= hitbox.lifetime)
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
} // namespace app::system