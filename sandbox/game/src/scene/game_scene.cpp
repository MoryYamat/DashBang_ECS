#include "game/scene/game_scene.h"

#include <ddknd/asset/asset_manager.h>
#include <ddknd/ecs/ecs.h>

#include <ddknd/graphics/gfx_asset_loader.h>

#include "game/assets/actor/paladin_assets.h"
#include "game/camera/game_camera.h"
#include "game/player/player.h"
#include "game/player/player_controller.h"

#include <cassert>

namespace app::scene
{
    GameScene CreateMainScene(::ddknd::ecs::World& world, ::ddknd::asset::AssetManager& assetMgr)
    {
        auto paladinAssets = ::app::assets::actor::RegisterPaladinAssets(assetMgr);

        auto player = ::app::player::CreatePaladinPlayer(
            world, paladinAssets, ::app::player::PlayerSpawnDesc{.position = {0.0f, 0.0f, 0.0f}, .moveSpeed = 5.0f});

        auto mainCamera = ::app::camera::CreateCameraEntity(world, ::app::camera::CameraEntityDesc{});

        auto cameraRig = ::app::camera::CreatePlayerCameraRig(
            world, ::app::camera::PlayerCameraRigSpawnDesc{.target = player,
                                                           .camera = mainCamera,
                                                           .lookOffset = {0.0f, 2.0f, 0.0f},
                                                           .yawDeg = -90.0f,
                                                           .pitchDeg = 25.0f,
                                                           .distance = 5.0f});

        auto controller = app::player::CreateLocalPlayerController(
            world, ::app::player::PlayerControllerSpawnDesc{.actor = player, .cameraRig = cameraRig});

        GameSceneAssets assets{.paladin = paladinAssets};
        GameSceneEntities entities{.player = player, .mainCamera = mainCamera};

        return GameScene{.assets = assets, .entities = entities};
    }

    bool LoadMainSceneAssets(const GameSceneAssets& assets, SceneLoadContext& ctx)
    {
        assert(ctx.assetManager);
        assert(ctx.graphicsLoader);
        assert(ctx.graphicsStore);
        assert(ctx.animationStore);

        bool ok = true;

        ok &= ctx.graphicsLoader->LoadShader(*ctx.assetManager, *ctx.graphicsStore, assets.paladin.skinnedShader);
        ok &= ctx.graphicsLoader->LoadModel(*ctx.assetManager, *ctx.graphicsStore, *ctx.animationStore,
                                            assets.paladin.model);

        return ok;
    }
} // namespace app::scene