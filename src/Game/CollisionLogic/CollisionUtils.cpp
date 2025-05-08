#include "CollisionUtils.h"


#include <iostream>

bool CollisionUtils::CheckCircleAABBIntersection(
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

	float squareSum = dx * dx +dy * dy;

	//std::cout << "distance^2: " << distanceSquared << "\n";
	//std::cout << "dx*dx + dy*dy: " << squareSum << "\n";
	//std::cout << "diff: " << std::abs(distanceSquared - squareSum) << "\n";
	// std::cout << "[CollisionUtils.cpp(CheckCircleAABBIntersection)]: result. " << i << std::endl;

	// ���a�̂Q��Ɣ�r���Č�������
	return squareSum<=(radius * radius);
}