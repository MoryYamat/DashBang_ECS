// Folder for writing collision processing
// 

#pragma once 


#include <GLM/glm.hpp>

namespace CollisionUtils
{
	// A pure function that performs collision detection between a circle and an AABB.
	// ‰~‚Æ AABB‚ªŒğ·‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©‚ğ”»’è‚·‚é
	bool CheckCircleAABBIntersection(
		const glm::vec2& circleCenter,
		float radius,
		const glm::vec2& boxMin,
		const glm::vec2& boxMax
	);
}