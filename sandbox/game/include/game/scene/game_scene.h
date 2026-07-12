#pragma once

#include <ddknd/asset/asset_fwd.h>
#include <ddknd/ecs/ecs_fwd.h>
#include <ddknd/ecs/entity/entity.h> // required for ::ddknd::ecs::Entity
#include <ddknd/graphics/graphics_fwd.h>


#include "game/assets/actor/mutant_assets.h"
#include "game/assets/actor/paladin_assets.h"


#include <vector>

namespace app::scene
{
    // @brief Set of entities in GameScene.
    struct GameSceneEntities
    {
        ::ddknd::ecs::Entity player;
        ::ddknd::ecs::Entity mainCamera;

        std::vector<ddknd::ecs::Entity> npcs;
    };

    // @brief Set the AssetIDs of the Actors used for the Entity.
    struct GameSceneAssets
    {
        app::assets::actor::PaladinAssetIDs paladin;
        app::assets::actor::MutantAssetIDs mutant;

        [[nodiscard]]
        bool IsValid() const noexcept
        {
            if (!paladin.IsValid())
            {
                std::cerr << "[GameSceneAssets::IsValid] An InvalidID exists in the Paladin Asset ID.\n";
            }

            if (!mutant.IsValid())
            {
                std::cerr << "[GameSceneAssets::IsValid] An InvalidID exists in the Mutant Asset ID.\n";
            }
            return paladin.IsValid() && mutant.IsValid();
        }
    };

    struct GameScene
    {
        GameSceneAssets assets;
        GameSceneEntities entities;
    };

    GameScene CreateMainScene(::ddknd::ecs::World& world, ::ddknd::asset::AssetManager& assetMgr);

    struct SceneLoadContext
    {
        ::ddknd::asset::AssetManager* assetManager = nullptr;
        ::ddknd::graphics::GraphicsAssetLoader* graphicsLoader = nullptr;
        ::ddknd::graphics::GraphicsAssetStore* graphicsStore = nullptr;
        ::ddknd::animation::AnimationAssetStore* animationStore = nullptr;
    };

    bool LoadMainSceneAssets(const GameSceneAssets& assets, SceneLoadContext& ctx);
} // namespace app::scene