#include "PlayerTileCollisionExtractor.h"

#include "Engine/Physics/Private/Logic2D/DetectionFunctions.h"

#include "Game/Collision/Utils/ExtractSupport.h"

#include "Engine/Debug/Private/DebugUtils.h"

#include <iostream>

std::vector<glm::ivec2> Game::Collision::Extract::ExtractPlayerTileCollisions(
	const Game::Collision::Data::PlayerCollisionContext& playerCollisionCtx,
	const Engine::ECS::Component::Logic2D::TileMapComponent& tileMapComp
)
{
	std::vector<glm::ivec2> hitTiles;

	glm::ivec4 tileSearchBounds = Game::Collision::Utils::GetTileSearchBounds(playerCollisionCtx, tileMapComp);

	// std::cout << "Bounds: row(" << tileSearchBounds.x << " to " << tileSearchBounds.y << "), col(" << tileSearchBounds.z << " to " << tileSearchBounds.w << ")" << std::endl;

	int colStart = tileSearchBounds.x;
	int colEnd = tileSearchBounds.y;
	int rowStart = tileSearchBounds.z;
	int rowEnd = tileSearchBounds.w;


	for (int row = rowStart; row <= rowEnd; ++row)
	{
		for (int col = colStart; col <= colEnd; ++col)
		{
			auto [tileMin, tileMax] = tileMapComp.GetTileAABB(row, col);

			// Detect
			if (Engine::Physics::Logic2D::Collision::CheckCircleAABBIntersection(playerCollisionCtx.center, playerCollisionCtx.radius, tileMin, tileMax))
			{
				// DebugUtils::LogVector_string("[PlayerTileCollisionExtractor.cpp]: TileMin.", tileMin);
				// DebugUtils::LogVector_string("[PlayerTileCollisionExtractor.cpp]: TileMax.", tileMax);
				hitTiles.emplace_back(glm::ivec2(col, row));
			}
		}
	}

	return hitTiles;
}