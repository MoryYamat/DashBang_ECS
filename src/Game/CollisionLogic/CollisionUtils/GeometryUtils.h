// Purely mathematical collision detection (logic of "hit")
// �����Ȑ��w�I�Փ˔���i���W�b�N�Ƃ��Ắu������v�j


#pragma once

#include "Core/ECS/Component/Collision/CollisionComponent.h"

#include <GLM/glm.hpp>

namespace GameUtils::CollisionLogic::GeometryUtils
{
	// A pure function that performs collision detection between a circle and an AABB.
	// �~�� AABB���������Ă��邩�ǂ����𔻒肷��
	bool CheckCircleAABBIntersection(
		const glm::vec2& circleCenter,
		float radius,
		const glm::vec2& boxMin,
		const glm::vec2& boxMax
	);

	bool intersectOBB2D_AABB2D(const Obb2D& obb, const glm::vec2& aabbMin, const glm::vec2& aabbMax);
}