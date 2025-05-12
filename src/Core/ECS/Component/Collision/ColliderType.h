// Define the name of ColliderType and data structure

#pragma once

#include <GLM/glm.hpp>

// name of ColliderType
enum class ColliderType
{
	None,
	Circle2D,
	Box2D,
	Obb2D,
	AABB3D,
	Sphere3D,
	Capsule3D,
};

// 2D円
struct Circle2D
{
	glm::vec2 center;
	float radius;
};

// 2D正方形
struct Box2D
{
	glm::vec2 center;
	glm::vec2 halfExtents;
};

// Oriented Bounding Box
// 回転あり境界ボックス
struct Obb2D
{
	glm::vec2 center;//
	glm::vec2 halfExtents; // (width/2, height/2)
	glm::vec2 axisX; // X軸方向の単位ベクトル
	glm::vec2 axisZ;// axisに直交

	// 点が含まれるかどうかを判定
	bool contains(const glm::vec2& pointXZ) const
	{
		// 相対ベクトル
		glm::vec2 d = pointXZ - center;
		float dx = glm::dot(d, axisX);// ローカルx軸への投影
		float dz = glm::dot(d, axisZ);// ローカルz軸への投影

		// 投影の絶対値とモデル半径(XZそれぞれを)比較して判定
		return std::abs(dx) <= halfExtents.x && std::abs(dz) <= halfExtents.y;
	}
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


// Collider Data Structure
struct Collider
{
	ColliderType type;

	union
	{
		// sizeof (need to think about bottlenecks)
		Circle2D circle2D;
		Box2D box2D;
		Obb2D obb2D;
		AABB3D aabb3D;
		Sphere3D sphere3D;
		Capsule3D capsule3D;
	};

	// default Constructor
	Collider() : type(ColliderType::None), box2D{} {}
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