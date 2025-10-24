#pragma once

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMInterferenceRequestComponent.hpp"

namespace Game::Combat::Skill::API::External
{
	inline Game::Combat::Skill::FSM::StateModel::SkillFSMInterferenceRequestComponent* 
		tryGetSkillInterferenceRequestComponent(Engine::ECS::EntityMgr& ecs, const Engine::ECS::Entity e
		)
	{
		if (ecs.hasComponent<Game::Combat::Skill::FSM::StateModel::SkillFSMInterferenceRequestComponent>(e))
		{
			return &ecs.get<Game::Combat::Skill::FSM::StateModel::SkillFSMInterferenceRequestComponent>(e);
		}

		return nullptr;
	}
}