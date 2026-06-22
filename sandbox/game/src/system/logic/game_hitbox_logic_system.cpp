#include "game/system/logic/game_hitbox_logic_system.h"

#include "game/component/character_stats_component.h"

#include <ddknd/component/general_component.h>

#include "game/component/state_component.h"
#include <ddknd/component/gfx_component.h>
#include <ddknd/component/hitbox_component.h>
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

            reg.AddComponent<ddknd::component::LifetimeComponent>(
                hitbox, ddknd::component::LifetimeComponent{.lifetime = def->hitbox.lifetime, .elapsed = 0.0f});

            ddknd::component::TransformComponent hitboxTransform{};
            hitboxTransform.localTRS.translation =
                ddknd::math::TransformPoint(transform->worldMatrix, def->hitbox.localOffset);
            hitboxTransform.worldMatrix = hitboxTransform.localTRS.ToMatrix();
            hitboxTransform.dirty = false;
            reg.AddComponent<ddknd::component::TransformComponent>(hitbox, hitboxTransform);

            // hitbox
            reg.AddComponent<ddknd::component::HitboxComponent>(
                hitbox,
                ddknd::component::HitboxComponent{.owner = owner});

            reg.AddComponent<ddknd::component::HemisphereHitboxComponent>(
                hitbox, ddknd::component::HemisphereHitboxComponent{.radius = def->hitbox.radius});
            state->hitboxSpawned = true;
        }
    }

    void HitboxCollisionSystem::Update(ddknd::ecs::World& world)
    {
        using namespace ddknd::ecs;
        auto& reg = world.GetRegistry();

        auto hitboxes =
            reg.view(query()
                         .select<ddknd::component::HitboxComponent>()
                         .require<ddknd::component::HemisphereHitboxComponent, ddknd::component::TransformComponent>())
                .withEntity();
        auto hurtboxes =
            reg.view(query()
                         .select<ddknd::component::HurtboxComponent>()
                         .require<ddknd::component::SphereHurtboxComponent, ddknd::component::TransformComponent>())
                .withEntity();
        for (auto [hitboxEntity, hitbox, hemi, hitboxTransform] : hitboxes)
        {
            for (auto [target, hurtbox, sphere, targetTransform] : hurtboxes)
            {
                if (target == hitbox.owner)
                {
                    continue;
                }
                std::cerr << "here\n";
                // detection
            }
        }
    }
} // namespace app::system