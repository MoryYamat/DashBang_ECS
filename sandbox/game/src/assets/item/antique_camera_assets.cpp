#include "game/assets/item/antique_camera_assets.h"

#include <ddknd/ecs/ecs.h>
#include <ddknd/asset/asset_manager.h>

#include <ddknd/component/gfx_component.h>

namespace app::assets::item
{
    AntiqueCameraAssetsIDs RegisterAntiqueCameraAssets(::ddknd::asset::AssetManager& assetMgr)
    {
        return AntiqueCameraAssetsIDs{
            .model = assetMgr.GetOrCreate<ddknd::asset::tag::Model>(AntiqueCameraAssets::kModelPath),
            .shader = assetMgr.GetOrCreate<ddknd::asset::tag::Shader>(AntiqueCameraAssets::kSkinnedShaderPath)
        };
    }

    ::ddknd::ecs::Entity CreateAntiqueCamera(::ddknd::ecs::World& world, const AntiqueCameraAssetsIDs& assets)
    {
        const auto e = world.Create();
        auto& registry = world.GetRegistry();

        // Spatial State
        auto& transform = registry.AddComponent<ddknd::component::TransformComponent>(e);
        transform.localTRS.translation = {0.0f, 0.0f, 0.0f};
        transform.localTRS.rotation = ::ddknd::math::Quatf::Identity();
        transform.dirty = true;

        // Rendering Resources
        registry.AddComponent<ddknd::component::ModelComponent>(e, ddknd::component::ModelComponent{.model = assets.model});
        registry.AddComponent<ddknd::component::MaterialComponent>(e, ddknd::component::MaterialComponent{.shader = assets.shader});

        return e;
    }
}