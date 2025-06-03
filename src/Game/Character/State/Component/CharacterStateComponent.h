// 

#pragma once

#include "Game/Character/State/Behavior/CharacterState.h"

#include <optional>

namespace Game::Character::State
{
	struct CharacterStateComponent
	{
		CharacterBehaviorState current = CharacterBehaviorState::Idle;// 現在の状態
		CharacterBehaviorState previous = CharacterBehaviorState::Idle;// 

		CharacterCrowdControlState crowdControl = CharacterCrowdControlState::None;

		float timeInState = 0.0f; // 経過時間．遷移判断などに使
		bool canBeInterrupted = true; // 中断可否フラグ


		// 状態の遷移要求(オプション：システム間の非同期リクエスト用)
		// 状態の遷移要求(オプション：システム間の非同期リクエスト用)
		std::optional<CharacterBehaviorState> requestedNextState;

		// キャラクターが行動制限状態かどうか
		inline bool IsCrowdControlled(CharacterCrowdControlState s)
		{
			return s != CharacterCrowdControlState::None;
		}

		// キャラクターが行動可能状態かどうか
		inline bool CanPerformAction(const CharacterStateComponent& c)
		{
			return c.current != CharacterBehaviorState::Dead &&
				c.crowdControl == CharacterCrowdControlState::None;
		}
	};
}