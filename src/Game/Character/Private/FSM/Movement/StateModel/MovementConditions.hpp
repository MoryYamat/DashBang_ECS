#pragma once

#include "MovementFSMContext.hpp"
#include <GLM/glm.hpp>

namespace Game::Character::FSM::Movement
{
	struct IMovementCondition
	{
		virtual ~IMovementCondition() = default;
		virtual bool evaluate(const MovementFSMContext& ctx) const = 0;
	};

	struct IsMoveIntentActive : IMovementCondition
	{
		bool evaluate(const MovementFSMContext& ctx) const override
		{
			return ctx.intentActive && glm::length(ctx.direction) > 0.001f;
		}
	};

	struct IsStopIntent : IMovementCondition
	{
		bool evaluate(const MovementFSMContext& ctx) const override
		{
			return !ctx.intentActive || glm::length(ctx.direction) <= 0.001f;
		}
	};


	// ˆÈ‰ºíœ—\’èFMovementFSM‚ğŠÏ‘ªŒ^‚©‚çí’“Œ^‚Ö•ÏX‚·‚é‚½‚ß”p~—\’è
	// ˆÚ“®‚ğŠJn‚Å‚«‚éğŒ
	struct CanMove : IMovementCondition
	{
		bool evaluate(const MovementFSMContext& ctx) const override
		{
			return glm::length(ctx.velocity) > 0.01f; // ‘¬“x‚ªƒ[ƒ‚Å‚È‚¢ê‡‚ÉˆÚ“®‰Â”\
		}
	};

	// ’â~‚·‚×‚«ğŒF‘¬“x‚ª‚Ù‚Úƒ[ƒ
	struct ShouldStop : IMovementCondition
	{
		bool evaluate(const MovementFSMContext& ctx) const override
		{
			return glm::length(ctx.velocity) <= 0.01f; // ‘¬“x‚ª‚Ù‚Úƒ[ƒ‚Ìê‡‚É’â~
		}
	};
}