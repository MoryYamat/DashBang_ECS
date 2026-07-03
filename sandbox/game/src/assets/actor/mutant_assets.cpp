#include "game/assets/actor/mutant_assets.h"


#include <ddknd/ecs/ecs.h>
#include <ddknd/asset/sub_asset_key.h>

#include <ddknd/component/gfx_component.h>

#include "game/component/animation_component.h"

namespace app::assets::actor
{
    MutantAssetIDs RegisterMutantAssets(::ddknd::asset::AssetManager& assetMgr)
    {
        using AnimationClip = ::ddknd::asset::tag::AnimationClip;
        return MutantAssetIDs{
            .model = assetMgr.GetOrCreate<::ddknd::asset::tag::Model>(MutantAssets::kModelPath),
            .skinnedShader = assetMgr.GetOrCreate<::ddknd::asset::tag::Shader>(MutantAssets::kSkinnedShaderPath),
            .idle = assetMgr.GetOrCreate<AnimationClip>(::ddknd::asset::MakeAnimationClipKey(MutantAssets::kModelPath, MutantAssets::kIdleClipName)),
        };
    }

    ::ddknd::ecs::Entity CreateMutant(::ddknd::ecs::World& world, const MutantAssetIDs& assets)
    {
        auto e = world.Create();
        auto& registry = world.GetRegistry();

        // Transform
        auto& transform = registry.AddComponent<::ddknd::component::TransformComponent>(e);
        transform.localTRS.translation = {0.0f, 0.0f, 0.0f};
        transform.localTRS.rotation = ::ddknd::math::Quatf::Identity();
        transform.dirty = true;

        // Model
        registry.AddComponent<::ddknd::component::SkinnedModelComponent>(e, ::ddknd::component::SkinnedModelComponent{.model = assets.model});

        // Render Reference
        registry.AddComponent<::ddknd::component::MaterialComponent>(e, ::ddknd::component::MaterialComponent{.shader = assets.skinnedShader});

        // Runtime animation state
        registry.AddComponent<::ddknd::component::PoseComponent>(e);
        registry.AddComponent<::ddknd::component::AnimationPlaybackComponent>(e);

        // ============== animation section ==============
        registry.AddComponent<app::component::CharacterAnimationClipsComponent>(
            e, app::component::CharacterAnimationClipsComponent{
                   .idle = assets.idle,
               });

        return e;
    }
}