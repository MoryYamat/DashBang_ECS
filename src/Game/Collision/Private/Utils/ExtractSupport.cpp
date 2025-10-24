#include "ExtractSupport.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"


#include "Engine/Debug/Private/DebugUtils.h"


glm::ivec4 Game::Collision::Utils::GetTileSearchBounds(
	const Game::Collision::Data::PlayerCollisionContext& playerCollisionCtx,
	const Engine::ECS::Component::Logic2D::TileMapComponent& tileMapComp
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