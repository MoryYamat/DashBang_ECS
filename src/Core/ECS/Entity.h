#pragma once

#include <cstdint>

// �R���p�C�����Ɍ���@(4294967295)
constexpr uint32_t INVALID_ENTITY_ID = static_cast<uint32_t>(-1);


// 4 Byte
struct Entity
{
	uint32_t id;

	// ���Z�q�I�[�o�[���[�h
	bool operator == (const Entity& other) const
	{
		return id == other.id;
	}
	// overload
	bool operator != (const Entity& other) const
	{
		return id != other.id;
	}

	static const Entity INVALID;

	static constexpr uint32_t INVALID_ENTITY_ID = static_cast<uint32_t>(-1);

};

inline const Entity Entity::INVALID = { Entity::INVALID_ENTITY_ID };