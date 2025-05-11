#include "PlayerTileCollisionExtractor.h"

#include "Game/CollisionLogic/CollisionUtils/ExtractSupport.h"
#include "Game/CollisionLogic/CollisionUtils/GeometryUtils.h"

#include "Debug/DebugUtils.h"

#include <iostream>

std::vector<glm::ivec2> GameUtils::CollisionLogic::Extractor::ExtractPlayerTileCollisions(
	const GameUtils::CollisionLogic::Data::PlayerCollisionContext& playerCollisionCtx,
	const TileMapComponent& tileMapComp
)
{
	std::vector<glm::ivec2> hitTiles;

	glm::ivec4 tileSearchBounds = GameUtils::CollisionLogic::ExractSupport::GetTileSearchBounds(playerCollisionCtx, tileMapComp);

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

			if (GameUtils::CollisionLogic::GeometryUtils::CheckCircleAABBIntersection(playerCollisionCtx.center, playerCollisionCtx.radius, tileMin, tileMax))
			{
				// DebugUtils::LogVector_string("[PlayerTileCollisionExtractor.cpp]: TileMin.", tileMin);
				// DebugUtils::LogVector_string("[PlayerTileCollisionExtractor.cpp]: TileMax.", tileMax);
				hitTiles.emplace_back(glm::ivec2(col, row));
			}
		}
	}

	return hitTiles;
}