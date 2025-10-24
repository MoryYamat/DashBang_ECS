// ECSやComponentから必要情報を抽出・変換する補助関数
// Auxiliary functions to extract and convert necessary information from ECS and Component

#pragma once

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"
#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Game/Collision/Private/Data/CollisionContextData.h"

#include "Game/Collision/Private/Component/CollisionMaskComponent.h"

#include <GLM/glm.hpp>

#include <utility>
#include <optional>



namespace Game::Collision::Utils
{
	// A function that uses information from the TileMapComponent to find the index range (rows and columns) of tiles that can intersect with the circle.
	// 円と交差しうるタイルのインデックス範囲（行・列）をTileMapComponentの情報を使って求める関数
	glm::ivec4 GetTileSearchBounds(
		const Game::Collision::Data::PlayerCollisionContext& playerCollisionCtx,
		const Engine::ECS::Component::Logic2D::TileMapComponent& tileMapComp
	);


}