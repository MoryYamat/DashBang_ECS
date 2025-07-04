// interface

#pragma once

namespace Game::Common::Logic::Condition
{
	// Šî’êƒNƒ‰ƒX 
	struct ICondition
	{
		virtual ~ICondition() = default;
		virtual bool evaluate(const RuntimeContext& ctx) const = 0;
	};
}