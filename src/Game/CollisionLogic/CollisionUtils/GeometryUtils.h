// Purely mathematical collision detection (logic of "hit")
// �����Ȑ��w�I�Փ˔���i���W�b�N�Ƃ��Ắu������v�j


#pragma once

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
}