#pragma once

#include <ddknd/ecs/ecs_fwd.h>

#include <ddknd/asset/asset_fwd.h>
#include <ddknd/asset/asset_id.h>
#include <ddknd/asset/asset_tag.h>

namespace app::assets::actor
{
    struct MutantAssets
    {
        static inline constexpr const char* kModelPath = "res://Models/mutant/mutant_idle.glb";
        static inline constexpr const char* kSkinnedShaderPath = "res://shaders/programs/skinned.shader";

        static inline constexpr const char* kIdleClipName = "idle_default";
    };

    struct MutantAssetIDs
    {
        ::ddknd::asset::AssetID<::ddknd::asset::tag::Model> model;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::Shader> skinnedShader;

        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> idle;

        [[nodiscard]]
        bool IsValid() const noexcept
        {
            return model.IsValid() && skinnedShader.IsValid() && idle.IsValid();
        }
    };

    // factory
    MutantAssetIDs RegisterMutantAssets(::ddknd::asset::AssetManager& assetMgr);
    ::ddknd::ecs::Entity CreateMutant(::ddknd::ecs::World& world, const MutantAssetIDs& assets);
} // namespace app::assets::actor