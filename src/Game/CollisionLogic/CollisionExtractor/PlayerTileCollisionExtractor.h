// Combining multiple collision-related utilities to create data
// コリジョンに関する複数のユーティリティを組み合わせてデータを作る

#pragma once

#include "Core/ECS/EntityManager.h"
#include "Core/ECS/Component/TileMapComponent.h"

#include "Game/CollisionLogic/Data/CollisionContextData.h"

#include <glm/glm.hpp>

namespace GameUtils::CollisionLogic::Extractor
{
	std::vector<glm::ivec2> ExtractPlayerTileCollisions(
		const GameUtils::CollisionLogic::Data::PlayerCollisionContext& playerCollisionCtx,
		const TileMapComponent& tileMapComp
	);
}