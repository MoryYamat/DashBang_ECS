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
		// ここが問題 
		// OR 判定 だから，どっちかに含まれていれば判定を行うように設定されている=> Layerを細分化する必要が出てきて，管理が問題になる
		// 一旦解決 => collisionMaskComponentの責務明確化 => collidesWithMaskでは，自分が能動的に衝突したい相手Layerだけ指定する
		// あとで構造的な解決を図る
		return (static_cast<uint32_t>(a.selfLayer) & b.collidesWithMask) ||
			(static_cast<uint32_t>(b.selfLayer) & a.collidesWithMask);
	}
}