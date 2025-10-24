#pragma once

#include "Game/Character/Private/FSM/CC/CCStateTags.hpp"

#include <cstdint>
#include <typeindex>
#include <optional>

namespace Game::Character::FSM::CC::StateModel
{
	// 
	struct CCFSMContext
	{
		
		std::optional<std::type_index> currentCC;
		float ccEnteredAt = 0.0f;					// CCに入った時刻(WorldClock基準)
		float ccDuration = 0.0f;					// now - ccEnteredAt(currentCCがある時だけ)

		bool immune = false;
		float immuneEndsAt = 0.0f;

		int chainCount = 0;	// 表示用
		float chainWindowStart = 0.0f;	// 
		
		std::optional<uint32_t> causeId = 0;

		// 削除予定
		// std::type_index dominantTag = StateTag::NONE;
		// bool hasActiveCC = false;
	};
}