#pragma once

#include "Game/Character/Control/CC/Component/CCAntiChainComponent.hpp"

#include "Game/Character/FSM/CC/StateModel/CCFSMContext.hpp"

namespace Game::Character::FSM::CC::API
{
	inline void updateContextFromAnti(
		Game::Character::FSM::CC::StateModel::CCFSMContext& ctx, 
		const Game::Character::Control::CC::Component::CCAntiChainComponent& anti)
	{
		ctx.chainCount = anti.count;
		ctx.immune = anti.immune;
		ctx.immuneEndsAt = anti.immuneUntil;
		ctx.chainWindowStart = anti.windowStart;
	}
}