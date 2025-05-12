#include "GeometryUtils.h"


#include <iostream>

bool GameUtils::CollisionLogic::GeometryUtils::CheckCircleAABBIntersection(
	const glm::vec2& circleCenter,
	float radius,
	const glm::vec2& boxMin,
	const glm::vec2& boxMax
)
{
	// AABB���ŉ~�̒��S�ɍł��߂��_�����߂�
	float nearestX = glm::clamp(circleCenter.x, boxMin.x, boxMax.x);
	float nearestY = glm::clamp(circleCenter.y, boxMin.y, boxMax.y);

	// ���̓_�Ɖ~�̒��S�̋����̂Q��
	float dx = circleCenter.x - nearestX;
	float dy = circleCenter.y - nearestY;

	// ���̕��@���ƁCroot���g���̂ŕ��������_�덷���g�債�Ă��܂��\��������
	//float distance = glm::distance(circleCenter, glm::vec2(nearestX, nearestY));
	//float distanceSquared = distance * distance;

	float squareSum = dx * dx + dy * dy;

	//std::cout << "distance^2: " << distanceSquared << "\n";
	//std::cout << "dx*dx + dy*dy: " << squareSum << "\n";
	//std::cout << "diff: " << std::abs(distanceSquared - squareSum) << "\n";
	// std::cout << "[CollisionUtils.cpp(CheckCircleAABBIntersection)]: result. " << i << std::endl;

	// ���a�̂Q��Ɣ�r���Č�������
	return squareSum <= (radius * radius);
}

// Separating Axis Theorem (SAT)
bool GameUtils::CollisionLogic::GeometryUtils::intersectOBB2D_AABB2D(const Obb2D& obb, const glm::vec2& aabbMin, const glm::vec2& aabbMax)
{
	// AABB�̒��S�ƃn�[�t�T�C�Y
	glm::vec2 aabbCenter = 0.5f * (aabbMin + aabbMax);
	glm::vec2 aabbHalfExtents = 0.5f * (aabbMax - aabbMin);

	// AABB�̎�(�Œ�)
	glm::vec2 aabbAxes[2] = {
		glm::vec2(1.0f, 0.0f),// X��
		glm::vec2(0.0f, 1.0f)// Z��
	};

	glm::vec2 obbAxes[2] = {
		obb.axisX,
		obb.axisZ
	};

	for (int i = 0; i < 2; ++i)
	{
		glm::vec2 axis = aabbAxes[i];

		// AABB�̓��e���a
		float aabbProjection = glm::dot(aabbHalfExtents, glm::abs(axis));

		// OBB�̓��e���a
		float obbProjection =
			obb.halfExtents.x * std::abs(glm::dot(obb.axisX, axis)) +
			obb.halfExtents.y * std::abs(glm::dot(obb.axisZ, axis));

		// ���S�Ԃ̃x�N�g��
		glm::vec2 centerDiff = obb.center - aabbCenter;
		float centerDist = std::abs(glm::dot(centerDiff, axis));

		if (centerDist > aabbProjection + obbProjection)
			return false;
	}

	for (int i = 0; i < 2; ++i)
	{
		glm::vec2 axis = obbAxes[i];

		float aabbProj = aabbHalfExtents.x * std::abs(glm::dot(glm::vec2(1, 0), axis)) +
			aabbHalfExtents.y * std::abs(glm::dot(glm::vec2(0, 1), axis));

		float obbProj =
			obb.halfExtents.x * std::abs(glm::dot(obb.axisX, axis)) +
			obb.halfExtents.y * std::abs(glm::dot(obb.axisZ, axis));

		glm::vec2 centerDiff = obb.center - aabbCenter;
		float centerDist = std::abs(glm::dot(centerDiff, axis));

		if (centerDist > aabbProj + obbProj)
			return false;
	}
	

	return true;// �S���ŕ����Ȃ�= �Փ�
}