// Combining multiple collision-related utilities to create data
// コリジョンに関する複数のユーティリティを組み合わせてデータを作る

#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Game/Collision/Data/CollisionContextData.h"

#include <glm/glm.hpp>

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Collision::Extract
{
	std::vector<glm::ivec2> ExtractPlayerTileCollisions(
		const gNsCollData::PlayerCollisionContext& playerCollisionCtx,
		const eNsLogic2DComp::TileMapComponent& tileMapComp
	);
}