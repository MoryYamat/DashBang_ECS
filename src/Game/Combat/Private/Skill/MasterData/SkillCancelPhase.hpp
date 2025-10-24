#pragma once

#include <cstdint>


namespace Game::Combat::Skill::Data
{
	enum class SkillCancelPhase : uint32_t
	{
		None = 0, // キャンセル不可
		DuringCast = 1 << 0, // キャスト中にキャンセル可能
		DuringActive = 1 << 1, // アクティブ中にキャンセル可能
		DuringRecovery = 1 << 2, // リカバリー中にキャンセル可能

		All = 0xFFFFFFFF // 全てのフェーズでキャンセル可能
	};

	// or演算
	inline SkillCancelPhase operator|(SkillCancelPhase lhs, SkillCancelPhase rhs)
	{
		return static_cast<SkillCancelPhase>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}

	// and演算
	inline SkillCancelPhase operator&(SkillCancelPhase lhs, SkillCancelPhase rhs)
	{
		return static_cast<SkillCancelPhase>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
	}

	// 不一致
	inline bool operator!=(SkillCancelPhase lhs, SkillCancelPhase rhs)
	{
		return static_cast<uint32_t>(lhs) != static_cast<uint32_t>(rhs);
	}

	// 省略記法
	inline SkillCancelPhase& operator|=(SkillCancelPhase& lhs, SkillCancelPhase rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	// ある`Phase`内でキャンセル可能かどうかをチェックする
	inline bool HasCancelPhase(SkillCancelPhase phase, SkillCancelPhase check)
	{
		return (phase & check) != SkillCancelPhase::None;
	}

}
