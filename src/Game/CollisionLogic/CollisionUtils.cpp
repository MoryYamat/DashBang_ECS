#include "CollisionUtils.h"


#include <iostream>

bool CollisionUtils::CheckCircleAABBIntersection(
	const glm::vec2& circleCenter,
	float radius,
	const glm::vec2& boxMin,
	const glm::vec2& boxMax
)
{
	// AABB“à‚Å‰~‚Ì’†S‚ÉÅ‚à‹ß‚¢“_‚ğ‹‚ß‚é
	float nearestX = glm::clamp(circleCenter.x, boxMin.x, boxMax.x);
	float nearestY = glm::clamp(circleCenter.y, boxMin.y, boxMax.y);

	// ‚»‚Ì“_‚Æ‰~‚Ì’†S‚Ì‹——£‚Ì‚Qæ
	float dx = circleCenter.x - nearestX;
	float dy = circleCenter.y - nearestY;

	// ‚±‚Ì•û–@‚¾‚ÆCroot‚ğg‚¤‚Ì‚Å•‚“®¬”“_Œë·‚ªŠg‘å‚µ‚Ä‚µ‚Ü‚¤‰Â”\«‚ª‚ ‚é
	//float distance = glm::distance(circleCenter, glm::vec2(nearestX, nearestY));
	//float distanceSquared = distance * distance;

	float squareSum = dx * dx +dy * dy;

	//std::cout << "distance^2: " << distanceSquared << "\n";
	//std::cout << "dx*dx + dy*dy: " << squareSum << "\n";
	//std::cout << "diff: " << std::abs(distanceSquared - squareSum) << "\n";
	// std::cout << "[CollisionUtils.cpp(CheckCircleAABBIntersection)]: result. " << i << std::endl;

	// ”¼Œa‚Ì‚Qæ‚Æ”äŠr‚µ‚ÄŒğ·”»’è
	return squareSum<=(radius * radius);
}