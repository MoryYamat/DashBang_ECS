//
#pragma once

#include "Engine/ECS/Component/Logic2D/ColliderType.h"
#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Game/Collision/Data/GenericShape2D.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

#include <glm/glm.hpp>

namespace Game::Collision::Convert
{
	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算
	gNsCollData::GenericShape2D ConvertFromCollider(const eNsLogic2DComp::Collider& collider
		, const eNsLogic2DComp::Logic2DTransformComponent& transform);

	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算
	gNsCollData::GenericShape2D ConvertFromAttackShape(const gNsSkillComp::Attack2DShape& attackShape
		, const eNsLogic2DComp::Transform2DComponent& worldTransform);

	// ローカル形状のオフセットを適用して中心座標のワールド変換を取得する
	glm::vec2 applyLocalToWorldPoint(const glm::vec2& localOffset, const eNsLogic2DComp::Logic2DTransformComponent& transfrom);

	// ローカル形状のオフセットを適用して中心座標のワールド変換を取得する
	glm::vec2 applyLocalToWorldPoint(const glm::vec2& localOffset, const eNsLogic2DComp::Transform2DComponent& transfrom);

	// ローカル形状の方向ベクトルをとワールド変換情報から方向ベクトルのワールド変換
	glm::vec2 transformLocalPointToWorld(const glm::vec2& localOffset, const eNsLogic2DComp::Logic2DTransformComponent& transfrom);

	// ローカル形状の方向ベクトルをとワールド変換情報から方向ベクトルのワールド変換
	glm::vec2 transformLocalPointToWorld(const glm::vec2& localOffset, const eNsLogic2DComp::Transform2DComponent& transfrom);
}