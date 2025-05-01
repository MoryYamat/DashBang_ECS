#pragma once

#include <cstdint>

// コンパイル時に決定　(4294967295)
constexpr uint32_t INVALID_ENTITY_ID = static_cast<uint32_t>(-1);

struct Entity
{
	uint32_t id;

	// 演算子オーバーロード
	bool operator == (const Entity& other) const
	{
		return id == other.id;
	}
};