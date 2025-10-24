#pragma once

#include "Engine/ECS/Entity.h"

#include "Game/Combat/Private/Skill/Def/SkillDef.hpp"

namespace Game::Combat::HitEvent::Data
{
	using EventID = uint32_t;

	struct HitEvent
	{

		EventID eventId = 0;

		Engine::ECS::Entity skill = Engine::ECS::Entity::INVALID;
		Engine::ECS::Entity target = Engine::ECS::Entity::INVALID;

		Game::Combat::Skill::Def::SkillID skillId = 0;

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