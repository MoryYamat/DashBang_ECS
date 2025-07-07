#pragma once

#include "MovementContext.hpp"
#include <GLM/glm.hpp>

namespace Game::Character::FSM::Movement
{
	struct IMovementCondition
	{
		virtual ~IMovementCondition() = default;
		virtual bool evaluate(const MovementContext& ctx) const = 0;
	};

	// 移動を開始できる条件
	struct CanMove : IMovementCondition
	{
		bool evaluate(const MovementContext& ctx) const override
		{
			return glm::length(ctx.velocity) > 0.01f; // 速度がゼロでない場合に移動可能
		}
	};

	// 停止すべき条件：速度がほぼゼロ
	struct ShouldStop : IMovementCondition
	{
		bool evaluate(const MovementContext& ctx) const override
		{
			return glm::length(ctx.velocity) <= 0.01f; // 速度がほぼゼロの場合に停止
		}
	};
}