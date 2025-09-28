#pragma once

#include <cstdint>

namespace Game::Character::FSM::CC::StateModel
{
	struct CCDedupStampComponent
	{
		uint32_t lastCauseId = 0;
	};
}