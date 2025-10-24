#pragma once

#include <unordered_map>
#include <unordered_set>

namespace Game::Character::FSM::CC::StateEffect
{
	struct CCStateEffectExecutionRecordComponent
	{
		// std::hashによるhash値を格納する
		std::unordered_set<std::size_t> executedStateEffectHashed;

		std::unordered_map<size_t, uint32_t> lastCauseByEffect;

		void markExecuted(std::size_t effectHash)
		{
			executedStateEffectHashed.insert(effectHash);
		};

		bool hasExecuted(std::size_t effectHash) const
		{
			return executedStateEffectHashed.count(effectHash) > 0;
		};

		void markExecutedWithCause(std::size_t effectHash, uint32_t causeId)
		{
			lastCauseByEffect[effectHash] = causeId;
		}

		bool hasExecutedWithCause(std::size_t effectHash, uint32_t causeId) const
		{
			auto it = lastCauseByEffect.find(effectHash);
			return (it != lastCauseByEffect.end() && it->second == causeId);
		}

		void clear()
		{
			executedStateEffectHashed.clear();
			lastCauseByEffect.clear();
		};
	};
}