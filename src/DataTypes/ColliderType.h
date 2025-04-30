// Define the name of ColliderType and data structure

#pragma once

#include <GLM/glm.hpp>


enum class ColliderType
{
	None,
	Circle2D,
	Box2D,
	AABB3D,
	Sphere3D,
	Capsule3D,
};

struct Circle2D
{
	glm::vec2 center;
	float radius;
};

struct Box2D
{
	glm::vec2 center;
	glm::vec2 halfExtents;
};

struct AABB3D
{
	glm::vec3 min;
	glm::vec3 max;
};

struct Sphere3D
{
	glm::vec3 center;
	float radius;
};

struct Capsule3D
{
	glm::vec3 base;
	glm::vec3 tip;
	float radius;
};

struct Collider
{
	ColliderType type;

	union
	{
		// sizeof (need to think about bottlenecks)
		Circle2D ciecle2D;
		Box2D box2D;

		AABB3D aabb3D;
		Sphere3D sphere3D;
		Capsule3D capsule3D;
	};
};


//// 2D
//enum class Collider2DType
//{
//	None,
//	Circle2D,
//	Box2D,
//};
//
//struct Collider2D
//{
//	Collider2DType type;
//	union
//	{
//		struct { glm::vec2 center; float radius; } circle;
//		struct { glm::vec2 center; glm::vec2 halfExtents; } box;
//	};
//};
//
//
//// 3D
//enum class Collider3DType
//{
//	None,
//	AABB3D,
//	Sphere3D,
//	Capsule3D,
//	Mesh3D,
//};
//
//struct Collider3DType
//{
//	Collider3DType type;
//	union
//	{
//		struct { glm::vec3 min; glm::vec3 max; } aabb;
//		struct { glm::vec3 center; float radius; } sphere;
//	};
//};


//memo
// Quadtree
// Octree
// Bounding volume hierarchy(BVH)
// Grid Hashing, Layer Musk, Broad Fase, narrow fase
//