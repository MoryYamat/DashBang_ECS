//
#pragma once

#include "Engine/ECS/Component/Logic2D/ColliderType.h"
#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"
#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Game/Collision/Data/GenericShape2D.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

#include <glm/glm.hpp>

namespace Game::Collision::Convert
{
	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算 (template実装のため廃止)
	gNsCollData::GenericShape2D ConvertFromCollider(const eNsLogic2DComp::Collider& collider
		, const eNsLogic2DComp::Logic2DTransformComponent& transform);

	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算 (tempalte実装のため廃止)
	gNsCollData::GenericShape2D ConvertFromAttackShape(const gNsSkillComp::Attack2DShape& attackShape
		, const eNsLogic2DComp::Transform2DComponent& worldTransform);

	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算
	gNsCollData::GenericShape2D BuildGenericShape2D(const eNsLogic2DComp::Collider& collider
		, const eNsLogic2DComp::Logic2DTransformComponent& transform);

	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算 
	gNsCollData::GenericShape2D BuildGenericShape2D(const gNsSkillComp::Attack2DShape& attack2DArea
		, const eNsLogic2DComp::Transform2DComponent& worldTransform);

	// (GenericShape2Dを作成)
	template<typename ShapeT, typename TransformT>
	gNsCollData::GenericShape2D MakeGenericShape2DFromTransform(
		const ShapeT& shape,
		const TransformT& transform
	)
	{
		return BuildGenericShape2D(shape, transform);
	}

	// (Transformの型が未知の場合)GenericShape2Dを作成するインターフェース
	template<typename ShapeT>
	gNsCollData::GenericShape2D MakeGenericShape2DAuto(
		const ShapeT& shape,
		eNsECS::Entity e,
		eNsECS::EntityMgr& ecs
	)
	{
		if (ecs.hasComponent<eNsLogic2DComp::Logic2DTransformComponent>(e)) {
			return MakeGenericShape2DFromTransform(shape, ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(e));
		}
		else if (ecs.hasComponent<eNsLogic2DComp::Transform2DComponent>(e)) {
			return MakeGenericShape2DFromTransform(shape, ecs.get<eNsLogic2DComp::Transform2DComponent>(e));
		}
		else {
			assert(false && "No valid transform component");
			return gNsCollData::GenericShape2D{};
		}
	}

	// GenericShape2Dを取得するインターフェース
	gNsCollData::GenericShape2D MakeGenericShape2D(
		eNsECS::Entity e,
		eNsECS::EntityMgr& ecs);

	// ローカル形状のオフセットを適用して中心座標のワールド変換を取得する
	glm::vec2 applyLocalToWorldPoint(const glm::vec2& localOffset, const eNsLogic2DComp::Logic2DTransformComponent& transform);

	// ローカル形状のオフセットを適用して中心座標のワールド変換を取得する
	glm::vec2 applyLocalToWorldPoint(const glm::vec2& localOffset, const eNsLogic2DComp::Transform2DComponent& transform);

	// ローカル形状の方向ベクトルをとワールド変換情報から方向ベクトルのワールド変換
	glm::vec2 transformLocalPointToWorld(const glm::vec2& localOffset, const eNsLogic2DComp::Logic2DTransformComponent& transform);

	// ローカル形状の方向ベクトルをとワールド変換情報から方向ベクトルのワールド変換
	glm::vec2 transformLocalPointToWorld(const glm::vec2& localOffset, const eNsLogic2DComp::Transform2DComponent& transform);
}