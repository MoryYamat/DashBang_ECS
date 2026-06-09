#include "game/assets/actor/paladin_assets.h"

#include <ddknd/ecs/ecs.h>
#include <ddknd/component/gfx_component.h>
#include <ddknd/math/math.h>

namespace app::assets::actor
{
    ::ddknd::ecs::Entity CreatePaladin(::ddknd::ecs::World& world, const PaladinAssetIDs& assets) 
    {
        using namespace ::ddknd;
        auto e = world.Create();
        auto& registry = world.GetRegistry();

        // Transform
        auto& transform = registry.AddComponent<component::TransformComponent>(e);
        transform.localTRS.translation = {0.0f, 0.0f, 0.0f};
        transform.localTRS.rotation = math::Quatf::Identity();
        transform.dirty = true;

        // Model
        registry.AddComponent<component::SkinnedModelComponent>(e, component::SkinnedModelComponent{.model = assets.model});

        // Render Reference
        registry.AddComponent<component::MaterialComponent>(e, component::MaterialComponent{.shader = assets.skinnedShader});

        // Runtime animation state
        registry.AddComponent<component::PoseComponent>(e);
        registry.AddComponent<component::AnimationPlaybackComponent>(e);
        
        return e;
    }
} // namespace app::assets::actor