#include "game/scene/game_scene.h"

#include <ddknd/asset/asset_manager.h>
#include <ddknd/ecs/ecs.h>

#include <ddknd/graphics/gfx_asset_loader.h>

#include "game/assets/actor/paladin_assets.h"
#include "game/assets/actor/mutant_assets.h"

#include "game/camera/game_camera.h"
#include "game/player/player.h"
#include "game/player/player_controller.h"

#include "game/actor/character/npc.h"

#include <cassert>
#include <iostream>

namespace app::scene
{
    GameScene CreateMainScene(::ddknd::ecs::World& world, ::ddknd::asset::AssetManager& assetMgr)
    {
        auto paladinAssets = ::app::assets::actor::RegisterPaladinAssets(assetMgr);
        auto mutantAssets = app::assets::actor::RegisterMutantAssets(assetMgr);

        auto player = ::app::player::CreatePaladinPlayer(
            world, paladinAssets, ::app::player::PlayerSpawnDesc{.position = {0.0f, 0.0f, 0.0f}, .moveSpeed = 2.0f});

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
        
        constexpr int spawnAmount = 1000;
        constexpr int columns = 40;
        constexpr float spacing = 1.5f;

        std::vector<ddknd::ecs::Entity> npcs;
        auto mutant_npc = app::actor::CreateMutantNPC(world, mutantAssets, app::actor::NPCSpawnDesc{.position = {0.0f, 0.0f, 5.0f}});
        npcs.push_back(mutant_npc);

        // **************** for character spawn test ****************
        // for(int i = 0; i < spawnAmount; i++)
        // {
        //     const int xIndex = i % columns;
        //     const int zIndex = i / columns;
        //     const float x = (static_cast<float>(xIndex) - static_cast<float>(columns - 1) * 0.5f) * spacing;
        //     const float z = static_cast<float>(zIndex) * spacing + 5.0f;
        //     auto mutant_npc = app::actor::CreateMutantNPC(world, mutantAssets, app::actor::NPCSpawnDesc{.position = {x, 0.0f, z}});
        //     npcs.push_back(mutant_npc);
        // }
        // **********************************************************
        

        GameSceneAssets assets{.paladin = paladinAssets, .mutant = mutantAssets};
        
        GameSceneEntities entities{.player = player, .mainCamera = mainCamera, .npcs = npcs};

        return GameScene{.assets = assets, .entities = entities};
    }

    bool LoadMainSceneAssets(const GameSceneAssets& assets, SceneLoadContext& ctx)
    {
        assert(ctx.assetManager);
        assert(ctx.graphicsLoader);
        assert(ctx.graphicsStore);
        assert(ctx.animationStore);

        bool ok = true;

        if(!assets.IsValid())
        {
            std::cerr << "Invalid GameSceneAssets\n";
            return false;
        }

        if(!ctx.graphicsLoader->LoadShader(*ctx.assetManager, *ctx.graphicsStore, assets.paladin.skinnedShader))
        {
            std::cerr << "Failed to load shader.\n";
            return false;
        }
        if(!ctx.graphicsLoader->LoadModel(*ctx.assetManager, *ctx.graphicsStore, *ctx.animationStore,
                                            assets.paladin.model))
        {
            std::cerr << "Failed to load Paladin Model\n";
            return false;
        }

        if(!ctx.graphicsLoader->LoadModel(*ctx.assetManager, *ctx.graphicsStore, *ctx.animationStore,
                                            assets.mutant.model))
        {
            std::cerr << "Failed to load Mutant Model\n";
            return false;
        }

        return true;
    }
} // namespace app::scene