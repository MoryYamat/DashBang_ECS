#include "game/actor/item/item.h"

#include <ddknd/ecs/ecs.h>

#include "game/assets/item/antique_camera_assets.h"

#include <ddknd/component/gfx_component.h>

namespace app::actor
{
    ddknd::ecs::Entity CreateAntiqueCameraItem(ddknd::ecs::World& world,
                                        const app::assets::item::AntiqueCameraAssetsIDs& antiqueCameraAssets,
                                        const ItemSpawnDesc& desc)
    {
        auto e = app::assets::item::CreateAntiqueCamera(world, antiqueCameraAssets);

        auto& reg = world.GetRegistry();
        auto& transform = reg.GetComponent<ddknd::component::TransformComponent>(e);
        transform.localTRS.translation = desc.position;
        transform.localTRS.scale = desc.scale;
        transform.dirty = true;


        // for debugging
        std::cerr << "antiqueCameraAssets: Entity = " << e.Index() << "\n";
        std::cerr << "antiqueCameraAssetsIDs: ModelID = " << antiqueCameraAssets.model.Value() << "\n";
        std::cerr << "antiqueCameraAssetsIDs: shader = " <<  antiqueCameraAssets.shader.Value() << "\n";
        return e;
    }
} // namespace app::actor