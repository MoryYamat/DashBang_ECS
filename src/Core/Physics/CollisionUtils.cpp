#include "CollisionUtils.h"

#include "Debug/DebugUtils.h"

#include <iostream>
#include <string>

// ŒÄ‚Ño‚µ‚ÍA`if(c.type == ColliderType::box2D) else {// –¢’è‹`“®ì}`
bool CollisionUtils::intersectBox2D(const Box2D& a, const Box2D& b)
{
	glm::vec2 aMin = a.center - a.halfExtents;
	glm::vec2 aMax = a.center + a.halfExtents;
	glm::vec2 bMin = b.center - b.halfExtents;
	glm::vec2 bMax = b.center + b.halfExtents;

	bool calc = (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
		(aMin.y <= bMax.y && aMax.y >= bMin.y);

	if (calc) DEBUG_LOG("Collision Detected at A(" +
		std::to_string(a.center.x) + "," + std::to_string(a.center.y) +
		") B(" + std::to_string(b.center.x) + "," + std::to_string(b.center.y) + ")");

	return calc;
}
