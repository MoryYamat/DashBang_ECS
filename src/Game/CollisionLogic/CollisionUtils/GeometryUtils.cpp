#include "GeometryUtils.h"


#include <iostream>

bool GameUtils::CollisionLogic::GeometryUtils::CheckCircleAABBIntersection(
	const glm::vec2& circleCenter,
	float radius,
	const glm::vec2& boxMin,
	const glm::vec2& boxMax
)
{
	// AABB内で円の中心に最も近い点を求める
	float nearestX = glm::clamp(circleCenter.x, boxMin.x, boxMax.x);
	float nearestY = glm::clamp(circleCenter.y, boxMin.y, boxMax.y);

	// その点と円の中心の距離の２乗
	float dx = circleCenter.x - nearestX;
	float dy = circleCenter.y - nearestY;

	// この方法だと，rootを使うので浮動小数点誤差が拡大してしまう可能性がある
	//float distance = glm::distance(circleCenter, glm::vec2(nearestX, nearestY));
	//float distanceSquared = distance * distance;

	float squareSum = dx * dx + dy * dy;

	//std::cout << "distance^2: " << distanceSquared << "\n";
	//std::cout << "dx*dx + dy*dy: " << squareSum << "\n";
	//std::cout << "diff: " << std::abs(distanceSquared - squareSum) << "\n";
	// std::cout << "[CollisionUtils.cpp(CheckCircleAABBIntersection)]: result. " << i << std::endl;

	// 半径の２乗と比較して交差判定
	return squareSum <= (radius * radius);
}

// Separating Axis Theorem (SAT)
bool GameUtils::CollisionLogic::GeometryUtils::intersectOBB2D_AABB2D(const Obb2D& obb, const glm::vec2& aabbMin, const glm::vec2& aabbMax)
{
	// AABBの中心とハーフサイズ
	glm::vec2 aabbCenter = 0.5f * (aabbMin + aabbMax);
	glm::vec2 aabbHalfExtents = 0.5f * (aabbMax - aabbMin);

	// AABBの軸(固定)
	glm::vec2 aabbAxes[2] = {
		glm::vec2(1.0f, 0.0f),// X軸
		glm::vec2(0.0f, 1.0f)// Z軸
	};

	glm::vec2 obbAxes[2] = {
		obb.axisX,
		obb.axisZ
	};

	for (int i = 0; i < 2; ++i)
	{
		glm::vec2 axis = aabbAxes[i];

		// AABBの投影半径
		float aabbProjection = glm::dot(aabbHalfExtents, glm::abs(axis));

		// OBBの投影半径
		float obbProjection =
			obb.halfExtents.x * std::abs(glm::dot(obb.axisX, axis)) +
			obb.halfExtents.y * std::abs(glm::dot(obb.axisZ, axis));

		// 中心間のベクトル
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
	

	return true;// 全軸で分離なし= 衝突
}