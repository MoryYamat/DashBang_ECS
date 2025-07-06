// interface

#pragma once

namespace Game::Common::Logic::FSM::Condition
{
	//
	template<typename Context>
	struct ICondition
	{
		virtual ~ICondition() = default;
		virtual bool evaluate(const Context& ctx) const = 0;
	};

	//// Šî’êƒNƒ‰ƒX 
	//struct ICondition
	//{
	//	virtual ~ICondition() = default;
	//	virtual bool evaluate(const RuntimeContext& ctx) const = 0;
	//};
}