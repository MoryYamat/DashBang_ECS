#pragma once

#include <unordered_set>

namespace Game::Character::FSM::CC::StateEffect
{
	struct CCStateEffectExecutionRecordComponent
	{
		// std::hash‚É‚æ‚éhash’l‚ðŠi”[‚·‚é
		std::unordered_set<std::size_t> executedStateEffectHashed;

		void markExecuted(std::size_t effectHash)
		{
			executedStateEffectHashed.insert(effectHash);
		};

		bool hasExecuted(std::size_t effectHash) const
		{
			return executedStateEffectHashed.count(effectHash) > 0;
		};

		void clear()
		{
			executedStateEffectHashed.clear();
		};
	};
}