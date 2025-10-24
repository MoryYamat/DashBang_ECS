#pragma once

#include <unordered_set>

namespace Game::Combat::Skill::Component
{
	struct SkillEffectExecutionRecordComponent
	{
		// std::hash‚É‚æ‚éhash’l‚ðŠi”[‚·‚é
		std::unordered_set<std::size_t> executedEffectHashed;

		void markExecuted(std::size_t effectHash)
		{
			executedEffectHashed.insert(effectHash);
		};

		bool hasExecuted(std::size_t effectHash) const
		{
			return executedEffectHashed.count(effectHash) > 0;
		};

		void clear()
		{
			executedEffectHashed.clear();
		}
	};
}