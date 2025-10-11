#pragma once


#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include "Game/Character/Animation/Profile/Skill/SkillAnimationProfile.hpp"

namespace Game::Character::Animation::Profile::Init
{
	inline void InitSkillAnimProfileDatabase(Engine::ECS::EntityMgr& ecs)
	{
		namespace Ops = Engine::ECS::Ops;
		namespace prof = Game::Character::Animation::Profile;
		using DB = Game::Character::Animation::Profile::SkillAnimationProfileDatabase;
		
		auto& db = Ops::CreateResouce<DB>(ecs);

		db.missingClip = "__MISSING_CLIP__";//

		// Default を用意（フォールバック連鎖の保険）
		//db.AddCharacter("Default", DB::SkillMap{
		//	{1u, "slash_outward_trim_default"}
		//	});


		//db.AddCharacter("Paladin", DB::SkillMap{
		//	{1u, "slash_outward_trim_default"}
		//	});

		db.AddCharacter("Default", DB::SkillMap{
			{1u, prof::SkillAnimMeta{ "slash_outward_trim_default", 1.0f, 0.0f, 0.0f, 0.0f, 0.0f}}
			});

	}
}