// ECSやComponentから必要情報を抽出・変換する補助関数
// Auxiliary functions to extract and convert necessary information from ECS and Component

#pragma once

#include "Core/ECS/Component/Collision/CollisionComponent.h"
#include "Core/ECS/Component/TileMapComponent.h"

#include "Game/CollisionLogic/Data/CollisionContextData.h"

#include <GLM/glm.hpp>

#include <utility>
#include <optional>

namespace GameUtils::CollisionLogic::ExractSupport
{
	// A function that uses information from the TileMapComponent to find the index range (rows and columns) of tiles that can intersect with the circle.
	// 円と交差しうるタイルのインデックス範囲（行・列）をTileMapComponentの情報を使って求める関数
	glm::ivec4 GetTileSearchBounds(
		const GameUtils::CollisionLogic::Data::PlayerCollisionContext& playerCollisionCtx,
		const TileMapComponent& tileMapComp
	);
}