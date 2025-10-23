//
#pragma once

#include "Engine/ECS/Component/Logic2D/ColliderType.h"
#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"
#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Game/Collision/Data/GenericShape2D.h"

#include "Engine/ECS/EntityManager.h"


#include <glm/glm.hpp>

namespace Game::Collision::Convert
{
	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算 (template実装のため廃止)
	Game::Collision::Data::GenericShape2D ConvertFromCollider(const Engine::ECS::Component::Logic2D::Collider& collider
		, const Engine::ECS::Component::Logic2D::Logic2DTransformComponent& transform);

	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算 (tempalte実装のため廃止)
	Game::Collision::Data::GenericShape2D ConvertFromAttackShape(const Game::Combat::Skill::Component::Attack2DShape& attackShape
		, const Engine::ECS::Component::Logic2D::Transform2DComponent& worldTransform);

	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算
	Game::Collision::Data::GenericShape2D BuildGenericShape2D(const Engine::ECS::Component::Logic2D::Collider& collider
		, const Engine::ECS::Component::Logic2D::Logic2DTransformComponent& transform);

	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算 
	Game::Collision::Data::GenericShape2D BuildGenericShape2D(const Game::Combat::Skill::Component::Attack2DShape& attack2DArea
		, const Engine::ECS::Component::Logic2D::Transform2DComponent& worldTransform);

	// (GenericShape2Dを作成)
	template<typename ShapeT, typename TransformT>
	Game::Collision::Data::GenericShape2D MakeGenericShape2DFromTransform(
		const ShapeT& shape,
		const TransformT& transform
	)
	{
		return BuildGenericShape2D(shape, transform);
	}

	// (Transformの型が未知の場合)GenericShape2Dを作成するインターフェース
	template<typename ShapeT>
	Game::Collision::Data::GenericShape2D MakeGenericShape2DAuto(
		const ShapeT& shape,
		Engine::ECS::Entity e,
		Engine::ECS::EntityMgr& ecs
	)
	{
		if (ecs.hasComponent<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e)) {
			return MakeGenericShape2DFromTransform(shape, ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e));
		}
		else if (ecs.hasComponent<Engine::ECS::Component::Logic2D::Transform2DComponent>(e)) {
			return MakeGenericShape2DFromTransform(shape, ecs.get<Engine::ECS::Component::Logic2D::Transform2DComponent>(e));
		}
		else {
			assert(false && "No valid transform component");
			return Game::Collision::Data::GenericShape2D{};
		}
	}

	// GenericShape2Dを取得するインターフェース
	Game::Collision::Data::GenericShape2D MakeGenericShape2D(
		Engine::ECS::Entity e,
		Engine::ECS::EntityMgr& ecs);

	// ローカル形状のオフセットを適用して中心座標のワールド変換を取得する
	glm::vec2 applyLocalToWorldPoint(const glm::vec2& localOffset, const Engine::ECS::Component::Logic2D::Logic2DTransformComponent& transform);

	// ローカル形状のオフセットを適用して中心座標のワールド変換を取得する
	glm::vec2 applyLocalToWorldPoint(const glm::vec2& localOffset, const Engine::ECS::Component::Logic2D::Transform2DComponent& transform);

	// ローカル形状の方向ベクトルをとワールド変換情報から方向ベクトルのワールド変換
	glm::vec2 transformLocalPointToWorld(const glm::vec2& localOffset, const Engine::ECS::Component::Logic2D::Logic2DTransformComponent& transform);

	// ローカル形状の方向ベクトルをとワールド変換情報から方向ベクトルのワールド変換
	glm::vec2 transformLocalPointToWorld(const glm::vec2& localOffset, const Engine::ECS::Component::Logic2D::Transform2DComponent& transform);
}