// Combining multiple collision-related utilities to create data
// コリジョンに関する複数のユーティリティを組み合わせてデータを作る

#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Game/Collision/Private/Data/CollisionContextData.h"

#include <glm/glm.hpp>




namespace Game::Collision::Extract
{
	std::vector<glm::ivec2> ExtractPlayerTileCollisions(
		const Game::Collision::Data::PlayerCollisionContext& playerCollisionCtx,
		const Engine::ECS::Component::Logic2D::TileMapComponent& tileMapComp
	);
}