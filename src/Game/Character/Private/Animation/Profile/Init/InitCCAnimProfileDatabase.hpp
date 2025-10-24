#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include "Game/Character/Private/Animation/Profile/CC/CCAnimationProfile.hpp"

#include "Game/Character/Private/FSM/CC/CCStateTags.hpp"

namespace Game::Character::Animation::Profile::Init
{
	namespace Ops = Engine::ECS::Ops;

	namespace CC = Game::Character::FSM::CC;

	using CCAnimProf = Game::Character::Animation::Profile::CC::CCAnimationProfile;
	using CCAnimProfDB = Game::Character::Animation::Profile::CC::CCAnimationProfileDatabase;

	inline void InitCCAnimProfileDatabase(Engine::ECS::EntityMgr& ecs)
	{
		auto& db = Ops::CreateResouce<CCAnimProfDB>(ecs);

		// 同じプロファイルは一回でまとめて登録すること
		db.AddCCProfile("default", CCAnimProfDB::CCMap{
			{CC::StateTag::STUNNED, CCAnimProf{.clipKey = "stunned_default", .speed = 1.0f, .fadeIn = 0.0f, .startOffset = 0.0f, .loop = true}},
			{CC::StateTag::KNOCKDOWNED, CCAnimProf{.clipKey = "falling_back_default", .speed = 1.0f, .fadeIn = 0.0f, .startOffset = 0.0f, .loop = false}},
			});
	}
}