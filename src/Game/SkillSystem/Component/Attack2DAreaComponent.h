// A component that defines the logical shape, trajectory, and lifetime of an attack.
// 攻撃判定の論理形状や軌跡，ライフタイムを定義するコンポーネント

#pragma once

#include <GLM/glm.hpp>

#include "Core/ECS/Entity.h"

#include "Game/Config/CanonicalDefaults.h"

#include <variant>

// 攻撃判定形状
// Attack Judgment Shape 
enum class Attack2DShapeType
{
	None,
	Circle2D,// 2D円
	Sector2D,// 2D扇形
	Rectangl2D,// 2D長方形
	Beam2D
};

// 2D円
struct Circle2DAttack
{
	glm::vec2 center = CanonicalDefaults::kLocalCenterXZ;
	float radius;
};

// 2D扇形
struct Sector2DAttack
{
	glm::vec2 center = CanonicalDefaults::kLocalCenterXZ;
	glm::vec2 direction = CanonicalDefaults::kLocalForwardXZ;// ローカル単位向きベクトル(OpenGLは-Z方向が正面)
	float angle;// ラジアン
	float radius;
};

// {center, width, height}
struct Rectangle2DAttack
{
	glm::vec2 center = CanonicalDefaults::kLocalCenterXZ;// local center
	glm::vec2 direction = CanonicalDefaults::kLocalForwardXZ; // ローカル単位向きベクトル(OpenGLは-Z方向が正面)
	float width;// right direction
	float height;// front direction
};

// 使用する形状を選択的に生成するインターフェースとなる構造体
// A structure that acts as an interface to selectively generate shapes to be used
struct Attack2DShape
{
	using VariantType = std::variant<Circle2DAttack, Sector2DAttack, Rectangle2DAttack>;
	VariantType shape;

	// helper functions
	bool IsCircle() const { return std::holds_alternative<Circle2DAttack>(shape); }
	bool IsSector() const { return std::holds_alternative<Sector2DAttack>(shape); }
	bool IsRectangle() const { return std::holds_alternative<Rectangle2DAttack>(shape); }

	Circle2DAttack& AsCircle() { return std::get<Circle2DAttack>(shape); }
	Sector2DAttack& AsSector() { return std::get<Sector2DAttack>(shape); }
	Rectangle2DAttack& AsRectoangle() { return std::get<Rectangle2DAttack>(shape); }

	const Circle2DAttack& AsCircle() const { return std::get<Circle2DAttack>(shape); }
	const Sector2DAttack& AsSector() const { return std::get<Sector2DAttack>(shape); }
	const Rectangle2DAttack& AsRectangle() const { return std::get<Rectangle2DAttack>(shape); }
};

// 
struct Attack2DAreaComponent
{
	Attack2DShape shape;
	float lifetime = 1.0f;
	Entity owner;// 攻撃者(キャラクター)
	Entity skillEntity;// この判定が属するスキルインスタンス
};