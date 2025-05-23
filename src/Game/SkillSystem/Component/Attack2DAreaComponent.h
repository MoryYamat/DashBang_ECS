// A component that defines the logical shape, trajectory, and lifetime of an attack.
// �U������̘_���`���O�ՁC���C�t�^�C�����`����R���|�[�l���g

#pragma once

#include <GLM/glm.hpp>

#include "Core/ECS/Entity.h"

#include "Game/Config/CanonicalDefaults.h"

#include <variant>

// �U������`��
// Attack Judgment Shape 
enum class Attack2DShapeType
{
	None,
	Circle2D,// 2D�~
	Sector2D,// 2D��`
	Rectangl2D,// 2D�����`
	Beam2D
};

// 2D�~
struct Circle2DAttack
{
	glm::vec2 center = CanonicalDefaults::kLocalCenterXZ;
	float radius;
};

// 2D��`
struct Sector2DAttack
{
	glm::vec2 center = CanonicalDefaults::kLocalCenterXZ;
	glm::vec2 direction = CanonicalDefaults::kLocalForwardXZ;// ���[�J���P�ʌ����x�N�g��(OpenGL��-Z����������)
	float angle;// ���W�A��
	float radius;
};

// {center, width, height}
struct Rectangle2DAttack
{
	glm::vec2 center = CanonicalDefaults::kLocalCenterXZ;// local center
	glm::vec2 direction = CanonicalDefaults::kLocalForwardXZ; // ���[�J���P�ʌ����x�N�g��(OpenGL��-Z����������)
	float width;// right direction
	float height;// front direction
};

// �g�p����`���I��I�ɐ�������C���^�[�t�F�[�X�ƂȂ�\����
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
	Entity owner;// �U����(�L�����N�^�[)
	Entity skillEntity;// ���̔��肪������X�L���C���X�^���X
};