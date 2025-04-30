//

#pragma once

#include <GLM/glm.hpp>

#include "DataTypes/ColliderType.h"

namespace CollisionUtils
{
	bool intersectCircle2D(const glm::vec2& c1, float r1, const glm::vec2& c2, float r2);

	bool intersectCircleBox2D(const glm::vec2& circleCenter, float radius,
		const glm::vec2& boxCenter, const glm::vec2& halfExtents);

	bool pointInCircle(const glm::vec2& p, const glm::vec2& c, float r);

	bool pointInBox(const glm::vec2& p, const glm::vec2& center, const glm::vec2& halfExtents);

	bool intersectBox2D(const Box2D& a, const Box2D& b);
};