#pragma once

#include <glm/glm.hpp>

#include <cmath>
#include <variant>
#include <cassert>

namespace Engine::Component
{


	enum class ColliderType
	{
		None,
		Circle2D,
		Box2D,
		Obb2D,
	};

	struct Circle2D
	{
		// ローカルオフセット
		glm::vec2 center;
		float radius;
	};

	// 2D正方形
	struct Box2D
	{
		// ローカルオフセット
		glm::vec2 center;
		glm::vec2 halfExtents;
	};

	// 回転あり境界ボックス
	struct Obb2D
	{
		// ローカルオフセット
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

	using CollideVariant = std::variant<std::monostate, Circle2D, Box2D, Obb2D>;


	// Collider Data Structure
	struct Collider
	{
		//Engine::ECS::Component::Logic2D::ColliderType type;

		// パフォーマンスと安全性を高める方法を考えるまではVariantで
		CollideVariant shape;

		ColliderType getType() const
		{
			return std::visit([](auto&& arg) -> ColliderType {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, Circle2D>) return ColliderType::Circle2D;
				else if constexpr (std::is_same_v<T, Box2D>) return ColliderType::Box2D;
				else if constexpr (std::is_same_v<T, Obb2D>) return ColliderType::Obb2D;
				else return ColliderType::None;
				}, shape);
		}

		// helper functions
		bool IsCircle2D() const { return std::holds_alternative<Circle2D>(shape); }
		bool IsBox2D() const { return std::holds_alternative<Box2D>(shape); }
		bool IsObb2D() const { return std::holds_alternative<Obb2D>(shape); }

		Circle2D& AsCircle2D() { return std::get<Circle2D>(shape); }
		Box2D& AsBox2D() { return std::get<Box2D>(shape); }
		Obb2D& AsObb2D() { return std::get<Obb2D>(shape); }

		const Circle2D& AsCircle2D() const
		{
			assert(IsCircle2D());
			return std::get<Circle2D>(shape);
		}
		const Box2D& AsBox2D() const
		{
			assert(IsBox2D());
			return std::get<Box2D>(shape);
		}
		const Obb2D& AsObb2D() const
		{
			assert(IsObb2D());// 開発時のみチェック
			return std::get<Obb2D>(shape);
		}

	};


	struct CollisionComponent
	{
		Collider collider;
		bool isStatic = true;
	};
}