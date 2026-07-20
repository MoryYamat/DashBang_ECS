#pragma once

#include <ddknd/ecs/ecs_fwd.h>

#include <ddknd/asset/asset_fwd.h>
#include <ddknd/asset/asset_id.h>
#include <ddknd/asset/asset_tag.h>

namespace app::assets::item
{
    struct AntiqueCameraAssets
    {
        static inline constexpr const char* kModelPath = "res://Models/items/AntiqueCamera.glb";
        static inline constexpr const char* kSkinnedShaderPath = "res://shaders/programs/mesh.shader";
    };

    struct AntiqueCameraAssetsIDs
    {
        ::ddknd::asset::AssetID<::ddknd::asset::tag::Model> model;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::Shader> shader;


        [[nodiscard]]
        bool IsValid() const noexcept
        {
            return  model.IsValid() &&
                    shader.IsValid();
        }

    };

    // factory
    AntiqueCameraAssetsIDs RegisterAntiqueCameraAssets(::ddknd::asset::AssetManager& assetMgr);
    ::ddknd::ecs::Entity CreateAntiqueCamera(::ddknd::ecs::World& world, const AntiqueCameraAssetsIDs& assets);

}