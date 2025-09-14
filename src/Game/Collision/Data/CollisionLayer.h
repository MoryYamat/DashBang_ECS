// collision layer attribute definition

#pragma once

#include <cstdint>

namespace Game::Collision::Data
{

	// bit flag
	enum class Layer : uint32_t
	{
		// レイヤーなし
		None = 0,

		// 相対的なレイヤー定義
		// Player
		Player = 1 << 0,
		// 敵
		Enemy = 1 << 1,
		// 味方
		Friendly = 1 << 2,
		// 中立
		Neutral = 1 << 3,


		// 
		// 相殺されないスキル
		Skill = 1 << 4,
		// 相殺対象になりうるスキル
		SkillCounterable = 1 << 5,
		// 相殺されないが当たるスキル?
		SkillUnblockable = 1 << 6,


		Tile = 1 << 7,
		Sensor = 1 << 8,


		All = 0xFF
	};

	// 演算子オーバーロード
	inline Layer operator|(Layer a, Layer b)
	{
		return static_cast<Layer>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}
}