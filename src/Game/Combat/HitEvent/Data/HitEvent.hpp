#pragma once

#include "Engine/ECS/Entity.h"

#include "Game/Combat/Skill/Def/SkillDef.hpp"

namespace Game::Combat::HitEvent::Data
{
	using namespace Engine::ECS;

	using namespace Game::Combat::Skill::Def;


	using EventID = uint32_t;

	struct HitEvent
	{

		EventID eventId = 0;

		Entity skill = Entity::INVALID;
		Entity target = Entity::INVALID;

		SkillID skillId = 0;

		float SpawnTime = 0.0f;

		bool CCExecutionFlag = false;


		bool HitEventCompletionFlag = false;
	};

	inline bool isCompleted(const HitEvent& e)
	{
		return e.HitEventCompletionFlag;
	}

	inline void markCompleted(HitEvent& e)
	{
		e.HitEventCompletionFlag = true;
	}

	inline void markCCExecuted(HitEvent& e)
	{
		e.CCExecutionFlag = true;
	}
}