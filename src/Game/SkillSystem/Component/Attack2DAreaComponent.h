// A component that defines the logical shape, trajectory, and lifetime of an attack.
// �U������̘_���`���O�ՁC���C�t�^�C�����`����R���|�[�l���g

#pragma once

#include <GLM/glm.hpp>

#include "Core/ECS/Entity.h"

#include <variant>

// �U������`��
// Attack Judgment Shape 
enum class Attack2DShapeType
{
	None,
	Circle2D,
	Sector2D,
	Rectangl2D,
	Beam2D
};

// 2D�~
struct Circle2DAttack
{
	glm::vec2 center;
	float radius;
};

// 2D��`
struct Sector2DAttack
{
	glm::vec2 center;
	glm::vec2 direction;// �P�ʃx�N�g��
	float angle;// ���W�A��
	float radius;
};

// �g�p����`���I��I�ɐ�������C���^�[�t�F�[�X�ƂȂ�\����
// A structure that acts as an interface to selectively generate shapes to be used
struct Attack2DShape
{
	using VariantType = std::variant<Circle2DAttack, Sector2DAttack>;
	VariantType shape;

	// helper functions
	bool IsCircle() const { return std::holds_alternative<Circle2DAttack>(shape); }
	bool IsSector() const { return std::holds_alternative<Sector2DAttack>(shape); }

	Circle2DAttack& AsCircle() { return std::get<Circle2DAttack>(shape); }
	Sector2DAttack& AsSector() { return std::get<Sector2DAttack>(shape); }

	const Circle2DAttack& AsCircle() const { return std::get<Circle2DAttack>(shape); }
	const Sector2DAttack& AsSector() const { return std::get<Sector2DAttack>(shape); }
};

// 
struct Attack2DAreaComponent
{
	Attack2DShape shape;
	float lifetime = 1.0f;
	Entity owner;// �U����(�L�����N�^�[)
	Entity skillEntity;// ���̔��肪������X�L���C���X�^���X
};