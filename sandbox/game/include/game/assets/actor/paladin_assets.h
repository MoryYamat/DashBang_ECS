#pragma once

#include <ddknd/ecs/ecs_fwd.h>

#include <ddknd/asset/asset_fwd.h>
#include <ddknd/asset/asset_id.h>
#include <ddknd/asset/asset_tag.h>

// @TODO need to reduce dependency
// #include <ddknd/graphics/gfx_type.h>

/*
animation clip の 名前についての契約

キャラクターロコモーションanimationの名前は(blenderでの作成時の段階で)以下のように統一する
The names for character locomotion animations must be standardized as follows (at the stage of creation in Blender):

- idle_default
- rolling_fwd_default
- run_back_default
- run_back_left_default
- run_back_right_default
- run_fwd_default
- run_fwd_left_default
- run_fwd_right_default
- run_left_default
- run_right_default

これらの編集などは(GameEngineRuntimeではなく)Editor機能として考えられる

These editing operations and the like can be considered Editor functions 
(rather than GameEngineRuntime functions).
*/

namespace app::assets::actor
{
    struct PaladinAssets
    {
        static inline constexpr const char* kModelPath = "res://Models/paladin/base_action_trimed.glb";
        // static inline constexpr const char* kModelPath = "res://Models/paladin/base_action_animation_diago.glb";
        static inline constexpr const char* kSkinnedShaderPath = "res://shaders/programs/skinned.shader";
        // static inline constexpr const char* kAttackDefaultAnimPath = "res://Models/paladin/attack_default.glb";// @TODO add import external animation file
        // animation clip logical path (internal clip)
        static inline constexpr const char* kIdleClipName = "idle_default";
        static inline constexpr const char* kRunForwardClipName = "run_fwd_default";
        static inline constexpr const char* kRunBackwardClipName = "run_back_default";
        static inline constexpr const char* kRunRightClipName = "run_right_default";
        static inline constexpr const char* kRunLeftClipName = "run_left_default";
        static inline constexpr const char* kRunRightForwardDiagonalClipName = "run_fwd_right_default";
        static inline constexpr const char* kRunLeftForwardDiagonalClipName = "run_fwd_left_default";
        static inline constexpr const char* kRunRightBackDiagonalClipName = "run_back_right_default";
        static inline constexpr const char* kRunLeftBackDiagonalClipName = "run_back_left_default";
        static inline constexpr const char* kAttackDefaultClipName = "attack_default";
    };

    struct PaladinAssetIDs
    {
        ::ddknd::asset::AssetID<::ddknd::asset::tag::Model> model;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::Shader> skinnedShader;

        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> idle;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runForward;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runBackward;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runRight;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runLeft;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runRightForwardDiagonal;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runLeftForwardDiagonal;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runRightBackDiagonal;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runLeftBackDiagonal;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> attack;


        [[nodiscard]]
        bool IsValid() const noexcept
        {
            return  model.Is_valid() &&
                    skinnedShader.Is_valid() &&
                    idle.Is_valid() &&
                    runForward.Is_valid() &&
                    runBackward.Is_valid() &&
                    runRight.Is_valid() &&
                    runLeft.Is_valid() &&
                    runRightForwardDiagonal.Is_valid() &&
                    runLeftForwardDiagonal.Is_valid() &&
                    runRightBackDiagonal.Is_valid() &&
                    runLeftBackDiagonal.Is_valid() &&
                    attack.Is_valid();
        }

    };

    // factory
    PaladinAssetIDs RegisterPaladinAssets(::ddknd::asset::AssetManager& assetMgr);
    ::ddknd::ecs::Entity CreatePaladin(::ddknd::ecs::World& world, const PaladinAssetIDs& assets);
} // namespace app::assets::actor