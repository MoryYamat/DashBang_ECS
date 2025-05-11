#include "ExtractSupport.h"

#include "Core/ECS/Component/PlayerControllerComponent.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"


#include "Debug/DebugUtils.h"


glm::ivec4 GameUtils::CollisionLogic::ExractSupport::GetTileSearchBounds(
	const GameUtils::CollisionLogic::Data::PlayerCollisionContext& playerCollisionCtx,
	const TileMapComponent& tileMapComp
)
{
	// 円の中心をTileMap上のローカル座標に変換
	glm::vec2 relative = playerCollisionCtx.center - tileMapComp.origin;

	// タイルインデックス範囲(付近のタイルだけ)
	int colStart = static_cast<int>(std::floor((relative.x - playerCollisionCtx.radius) / tileMapComp.tileSize)) - 1;
	int colEnd = static_cast<int>(std::ceil((relative.x + playerCollisionCtx.radius) / tileMapComp.tileSize)) + 1;
	int rowStart = static_cast<int>(std::floor((relative.y - playerCollisionCtx.radius) / tileMapComp.tileSize)) - 1;
	int rowEnd = static_cast<int>(std::ceil((relative.y + playerCollisionCtx.radius) / tileMapComp.tileSize)) + 1;

	// 範囲クランプ(安全にタイルアクセス)
	colStart = std::max(0, colStart);
	colEnd = std::min(tileMapComp.numCols - 1, colEnd);
	rowStart = std::max(0, rowStart);
	rowEnd = std::min(tileMapComp.numRows - 1, rowEnd);

	return glm::ivec4(colStart, colEnd, rowStart, rowEnd);
}