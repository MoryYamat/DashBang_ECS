// collision layer attribute definition

#pragma once

#include <cstdint>

namespace Game::Collision::Data
{
	// bit flag
	enum class Layer : uint8_t
	{
		None = 0,
		Player = 1 << 0,
		Enemy = 1 << 1,
		Tile = 1 << 2,
		Skill = 1 << 3,
		Sensor = 1 << 4,


		All = 0xFF
	};

	// 演算子オーバーロード
	inline Layer operator|(Layer a, Layer b)
	{
		return static_cast<Layer>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}
}