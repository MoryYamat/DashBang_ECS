// ECS��Component����K�v���𒊏o�E�ϊ�����⏕�֐�
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
	// �~�ƌ���������^�C���̃C���f�b�N�X�͈́i�s�E��j��TileMapComponent�̏����g���ċ��߂�֐�
	glm::ivec4 GetTileSearchBounds(
		const GameUtils::CollisionLogic::Data::PlayerCollisionContext& playerCollisionCtx,
		const TileMapComponent& tileMapComp
	);
}