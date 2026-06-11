#pragma once


#include <ddknd/ecs/ecs_fwd.h>
#include <ddknd/ecs/entity/entity.h>// required for ::ddknd::ecs::Entity
#include <ddknd/asset/asset_fwd.h>
#include <ddknd/graphics/graphics_fwd.h>

#include "game/assets/actor/paladin_assets.h"

namespace app::scene
{
    struct GameSceneEntities
    {
        ::ddknd::ecs::Entity player;
    };

    struct GameSceneAssets
    {
        app::assets::actor::PaladinAssetIDs paladin;
    };

    struct GameScene
    {
        GameSceneAssets assets;
        GameSceneEntities entities;
    };

    GameScene CreateMainScene
    (
        ::ddknd::ecs::World& world,
        ::ddknd::asset::AssetManager& assetMgr
    );

    struct SceneLoadContext
    {
        ::ddknd::asset::AssetManager* assetManager = nullptr;
        ::ddknd::graphics::GraphicsAssetLoader* graphicsLoader = nullptr;
        ::ddknd::graphics::GraphicsAssetStore* graphicsStore = nullptr;
        ::ddknd::animation::AnimationAssetStore* animationStore = nullptr;
    };

    bool LoadMainSceneAssets(
        const GameSceneAssets& assets,
        SceneLoadContext& ctx
    );
}