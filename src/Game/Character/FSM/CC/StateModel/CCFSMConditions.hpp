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

	struct NoCC : ICCFSMCondition
	{
		bool evaluate(const CCFSMContext& ctx) const override
		{
			return !ctx.currentCC.has_value();
		}
	};

	struct CurrentCCEquals : ICCFSMCondition
	{
		std::type_index tag;
		explicit CurrentCCEquals(std::type_index t): tag(t){}
		bool evaluate(const CCFSMContext& ctx) const override
		{
			return ctx.currentCC && *ctx.currentCC == tag;
		}
	};

	struct CCElapsedAtLeast : ICCFSMCondition
	{
		float sec;
		explicit CCElapsedAtLeast(float s) : sec(s) {}
		bool evaluate(const CCFSMContext& ctx) const override
		{
			return ctx.currentCC && ctx.ccDuration >= sec;
		}
	};

	struct ImmuneActive : ICCFSMCondition
	{
		bool evaluate(const CCFSMContext& ctx) const override
		{
			return ctx.immune;
		}
	};
}