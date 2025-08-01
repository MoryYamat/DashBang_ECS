#pragma once

#include "Game/Character/FSM/Movement/StateModel/MovementFSMContext.hpp"

namespace Game::Character::FSM::Movement::Effect
{

	// FIXME: 毎フレーム呼ばれる場合，効率問題が残る
	struct IEffectTriggerCondition
	{
		virtual ~IEffectTriggerCondition() = default;

		virtual bool evaluate(
			const MovementFSMContext& ctx,
			const std::type_index& currentState,
			const std::type_index& previousState
		) const = 0;
	};
}