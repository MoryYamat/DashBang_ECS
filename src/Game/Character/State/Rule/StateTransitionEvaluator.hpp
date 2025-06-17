//
#pragma once

#include "Game/Character/State/Component/CharacterStateComponent.h"
#include "Game/Character/State/Rule/StateTransitionRule.hpp"

namespace Game::Character::State
{
	class StateTransitionEvaluator
	{
	public:
		// ‘JˆÚğŒƒŠƒXƒg‚ª‚·‚×‚Ä–‚½‚³‚ê‚Ä‚¢‚é•]‰¿‚·‚é
		static bool AreConditionsMet
		(
			const CharacterStateComponent& stateComp,
			float deltaTime,
			const std::vector<TransitionCondition>& conditions
		);

	private:

		// ŠeğŒ‚ğŒÂ•Ê‚É•]‰¿‚·‚éƒwƒ‹ƒp[
		static bool EvaluateCondition(
			const CharacterStateComponent& stateComp,
			float deltaTime,
			const TransitionCondition& condition
		);
	};
}