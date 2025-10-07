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
			.runFwd = "run_default",
		};
	}
}