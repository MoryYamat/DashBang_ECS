#pragma once

#include "Game/Character/FSM/CC/CCStateTags.hpp"

#include <cstdint>
#include <optional>
#include <typeindex>

namespace Game::Character::FSM::CC::StateModel
{
	struct CCAppliedTransition
	{
		std::type_index fromState{ StateTag::NONE };
		std::type_index toState{ StateTag::NONE };
		float appliedAt{ 0.0f };
		bool isValid{ false };

		std::optional<uint32_t> causeId = 0;// hitevent の id
	};

	// TODO: typeidのパフォーマンス問題
	struct CCStateComponent
	{
		std::type_index previous;
		std::type_index current; // 現在の状態を表す型のインデックス

		std::type_index issuerId;// typeid(SkillFSMAxis), typeid(CCAxis), etc.

		float enteredAt = 0.0f;

		CCAppliedTransition applied{};
		uint32_t transitionSerial{ 0 };

		CCStateComponent() : current(StateTag::NONE), previous(StateTag::NONE) , issuerId(AxisTag::CCAxis), enteredAt(0.0f){} // 無効状態はvoid型

		explicit CCStateComponent(std::type_index iniState, std::type_index mainAxis)
			: current(iniState), previous(iniState) , issuerId(mainAxis) {
		} // 初期状態を指定して初期化

		inline void beginFrameSnapshot() { previous = current; }

		inline void applyTransition(
			std::type_index toState,
			float now
			)
		{
			const auto from = current;

			// 状態更新
			previous = from;
			current = toState;
			// issuerId = issuerAxis;

			if (current != StateTag::NONE && current != StateTag::IMMUNE)
			{
				enteredAt = now;
			}

			transitionSerial += 1;
			applied.fromState = from;
			applied.toState = current;
			applied.appliedAt = now;
			applied.isValid = true;
		}
	};
}