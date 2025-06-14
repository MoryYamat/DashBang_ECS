// ECSやComponentから必要情報を抽出・変換する補助関数
// Auxiliary functions to extract and convert necessary information from ECS and Component

#pragma once

#include "Game/Collision/Component/CollisionMaskComponent.h"

#include <GLM/glm.hpp>

#include "Common/GameNamespaceDecl.h"

namespace Game::Collision::Utils
{

	// 2つのエンティティが衝突判定すべきかを判定
	inline bool shouldCollide(
		const gNsCollComp::CollisionMaskComponent& a,
		const gNsCollComp::CollisionMaskComponent& b
	)
	{
		// a の selfLayer に対して b.collidesWithMaskが一致しているか
		return (static_cast<uint32_t>(a.selfLayer) & b.collidesWithMask) ||
			(static_cast<uint32_t>(b.selfLayer) & a.collidesWithMask);
	}
}