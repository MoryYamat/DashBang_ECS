// Õ“ËŒŸo‚Ì”Šw“IŒvZŠÖ”ŒQ

#pragma once

#include <GLM/glm.hpp>

#include "Engine/ECS/Component/Logic2D/ColliderType.h"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"

#include "Common/EngineNamespaceDecl.h"

namespace Engine::Physics::Logic2D::Collision
{
	bool intersectCircle2D(const glm::vec2& c1, float r1, const glm::vec2& c2, float r2);

	bool intersectCircleBox2D(const glm::vec2& circleCenter, float radius,
		const glm::vec2& boxCenter, const glm::vec2& halfExtents);

	bool pointInCircle(const glm::vec2& p, const glm::vec2& c, float r);

	bool pointInBox(const glm::vec2& p, const glm::vec2& center, const glm::vec2& halfExtents);

	// 
	bool intersectBox2D(const eNsLogic2DComp::Box2D& a, const eNsLogic2DComp::Box2D& b);


	// A pure function that performs collision detection between a circle and an AABB.
	// ‰~‚Æ AABB‚ªŒğ·‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©‚ğ”»’è‚·‚é
	bool CheckCircleAABBIntersection(
		const glm::vec2& circleCenter,
		float radius,
		const glm::vec2& boxMin,
		const glm::vec2& boxMax
	);

	bool intersectOBB2D_AABB2D(const eNsLogic2DComp::Obb2D& obb, const glm::vec2& aabbMin, const glm::vec2& aabbMax);
}

