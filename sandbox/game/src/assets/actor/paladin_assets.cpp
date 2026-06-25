#include "game/assets/actor/paladin_assets.h"

#include <ddknd/asset/asset_manager.h>
#include <ddknd/asset/asset_tag.h>

#include <ddknd/ecs/ecs.h>
#include <ddknd/component/gfx_component.h>
#include <ddknd/math/math.h>
#include <ddknd/asset/sub_asset_key.h>

#include "game/component/animation_component.h"

namespace app::assets::actor
{
    PaladinAssetIDs RegisterPaladinAssets(::ddknd::asset::AssetManager& assetMgr) 
    {
        using AnimationClip = ::ddknd::asset::tag::AnimationClip;
        return PaladinAssetIDs{
            .model = assetMgr.GetOrCreate<::ddknd::asset::tag::Model>(PaladinAssets::kModelPath),
            .skinnedShader = assetMgr.GetOrCreate<::ddknd::asset::tag::Shader>(PaladinAssets::kSkinnedShaderPath),

            .idle = assetMgr.GetOrCreate<AnimationClip>(::ddknd::asset::MakeAnimationClipKey(PaladinAssets::kModelPath, PaladinAssets::kIdleClipName)),
            .runForward = assetMgr.GetOrCreate<AnimationClip>(::ddknd::asset::MakeAnimationClipKey(PaladinAssets::kModelPath, PaladinAssets::kRunForwardClipName)),
            .runBackward = assetMgr.GetOrCreate<AnimationClip>(::ddknd::asset::MakeAnimationClipKey(PaladinAssets::kModelPath, PaladinAssets::kRunBackwardClipName)),
            .runRight = assetMgr.GetOrCreate<AnimationClip>(::ddknd::asset::MakeAnimationClipKey(PaladinAssets::kModelPath, PaladinAssets::kRunRightClipName)),
            .runLeft = assetMgr.GetOrCreate<AnimationClip>(::ddknd::asset::MakeAnimationClipKey(PaladinAssets::kModelPath, PaladinAssets::kRunLeftClipName)),
            .runRightForwardDiagonal = assetMgr.GetOrCreate<AnimationClip>(::ddknd::asset::MakeAnimationClipKey(PaladinAssets::kModelPath, PaladinAssets::kRunRightForwardDiagonalClipName)),
            .runLeftForwardDiagonal = assetMgr.GetOrCreate<AnimationClip>(::ddknd::asset::MakeAnimationClipKey(PaladinAssets::kModelPath, PaladinAssets::kRunLeftForwardDiagonalClipName)),
            .runRightBackDiagonal = assetMgr.GetOrCreate<AnimationClip>(::ddknd::asset::MakeAnimationClipKey(PaladinAssets::kModelPath, PaladinAssets::kRunRightBackDiagonalClipName)),
            .runLeftBackDiagonal = assetMgr.GetOrCreate<AnimationClip>(::ddknd::asset::MakeAnimationClipKey(PaladinAssets::kModelPath, PaladinAssets::kRunLeftBackDiagonalClipName)),
            .attack = assetMgr.GetOrCreate<AnimationClip>(::ddknd::asset::MakeAnimationClipKey(PaladinAssets::kModelPath, PaladinAssets::kAttackDefaultClipName)),
        };
    }

    ::ddknd::ecs::Entity CreatePaladin(::ddknd::ecs::World& world, const PaladinAssetIDs& assets) 
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
                   .runForward = assets.runForward,
                   .runBackward = assets.runBackward,
                   .runRight = assets.runRight,
                   .runLeft = assets.runLeft,
                   .runRightFowardDiagonal = assets.runRightForwardDiagonal,
                   .runLeftFowardDiagonal = assets.runLeftForwardDiagonal,
                   .runRightBackDiagonal = assets.runRightBackDiagonal,
                   .runLeftBackDiagonal = assets.runLeftBackDiagonal,
                   .attack = assets.attack,
               });

        return e;
    }
} // namespace app::assets::actor