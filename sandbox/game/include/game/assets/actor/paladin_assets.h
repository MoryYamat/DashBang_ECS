#pragma once

#include <ddknd/ecs/ecs_fwd.h>
#include <ddknd/asset/asset_manager.h>
#include <ddknd/asset/asset_tag.h>


namespace app::assets::actor
{
    struct PaladinAssets
    {
        static inline constexpr const char* kModelPath = "res://Models/paladin/base_action_animation_diago.glb";
        static inline constexpr const char* kSkinnedShaderPath = "res://shaders/programs/skinned.shader";
    };

    struct PaladinAssetIDs
    {
        ::ddknd::asset::AssetID<::ddknd::asset::tag::Model> model;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::Shader> skinnedShader;
    };

    inline PaladinAssetIDs RegisterPaladinAssets(::ddknd::asset::AssetManager& assetMgr) 
    {
        return PaladinAssetIDs{
            .model = assetMgr.GetOrCreate<::ddknd::asset::tag::Model>(PaladinAssets::kModelPath),
            .skinnedShader = assetMgr.GetOrCreate<::ddknd::asset::tag::Shader>(PaladinAssets::kSkinnedShaderPath),
        };
    }

    ::ddknd::ecs::Entity CreatePaladin(::ddknd::ecs::World& world, const PaladinAssetIDs& assets);
} // namespace app::assets::actor