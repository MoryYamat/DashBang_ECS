#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include "Game/Character/Animation/Profile/Movement/MovementAnimationProfile.hpp"

namespace Game::Character::Animation::Profile::Init
{
	inline void InitAnimationProfileDatabase(Engine::ECS::EntityMgr& ecs)
	{
		namespace Ops = Engine::ECS::Ops;
		namespace MvProfile = Game::Character::Animation::Profile;

		auto& mvdb = Ops::CreateResouce<MvProfile::MovementAnimationProfileDatabase>(ecs);

		mvdb.movements["PaladinDefault"] = MvProfile::MovementAnimationProfile
		{
			.idle = "idle_default",
			.runFwd = "run_fwd_default",
			.runBack = "run_back_default",
			.runRight = "run_right_default",
			.runLeft = "run_left_default",
			.runFwdLeft = "run_fwd_left_default",
			.runFwdRight = "run_fwd_right_default",
			.runBackLeft = "run_back_left_default",
			.runBackRight = "run_back_right_default",
		};
	}
}