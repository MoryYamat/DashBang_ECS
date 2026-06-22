#include "ddknd/system/hitbox_system.h"


#include <ddknd/ecs/ecs.h>
#include <ddknd/math/math.h>

#include <ddknd/system/system.h>

#include <ddknd/component/gfx_component.h>
#include <ddknd/component/hitbox_component.h>

#include <ddknd/event/hit_event.h>

namespace ddknd::system
{
    // ### TEMPORARY ### 
    void HitboxCollisionSystem::Update(ddknd::ecs::World& world, const ddknd::system::FrameContext& ctx)
    {
        assert(ctx.hitboxHitEvents && "HitboxCollisionSystem requires hitboxHitboxEvents.");
        
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
        for (auto [hitboxEntity, hitbox, hemi, hitboxTransform] : hitboxes)// semisphere
        {
            const auto hitboxCenter = ddknd::math::TransformPoint(hitboxTransform.worldMatrix, ddknd::math::Vec3f{0.0f, 0.0f, 0.0f});

            const auto forward = ddknd::math::normalize(ddknd::math::TransformDirection(hitboxTransform.worldMatrix, ddknd::math::Vec3f{0.0f,0.0f, 1.0f}));

            for (auto [target, hurtbox, sphere, targetTransform] : hurtboxes)//sphere
            {
                if (target == hitbox.owner)
                {
                    continue;
                }
                std::cerr << "here\n";

                // detection
                // ##################### TEMPORARY ##################### 
                const auto hurtboxCenter = ddknd::math::TransformPoint(targetTransform.worldMatrix, sphere.localOffset);
                const auto toTarget = hurtboxCenter - hitboxCenter;

                const float combinedRadius = hemi.radius + sphere.radius;
                const float distSq = ddknd::math::lengthSquared(toTarget);

                if(distSq > combinedRadius * combinedRadius)
                {
                    continue;
                }
                const float front = ddknd::math::dot(toTarget, forward);

                if(front < -sphere.radius)
                {
                    continue;
                }

                ctx.hitboxHitEvents->Push({.hitbox = hitboxEntity, .owner = hitbox.owner, .target = target});
            }
        }
    }
}