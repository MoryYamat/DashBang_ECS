#pragma once

#include "Game/Character/FSM/CC/StateModel/CCFSMContext.hpp"

#include <typeindex>

namespace Game::Character::FSM::CC::StateModel
{
	struct ICCFSMCondition
	{
		virtual ~ICCFSMCondition() = default;
		virtual bool evaluate(const CCFSMContext& ctx) const = 0;
	};

	struct AlwaysTrue :ICCFSMCondition
	{
		bool evaluate(const CCFSMContext&) const override
		{
			return true;
		}
	};

	struct DominantEquals : ICCFSMCondition
	{
		std::type_index tag;
		explicit DominantEquals(std::type_index t) : tag(t) {}
		bool evaluate(const CCFSMContext& ctx) const override
		{
			return ctx.dominantTag == tag;
		}
	};

	struct NoActiveCC : ICCFSMCondition
	{
		bool evaluate(const CCFSMContext& ctx) const override
		{
			return !ctx.hasActiveCC;
		}
	};
}