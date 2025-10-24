#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Character/Private/FSM/Interference/Core/Data/FSMInterferenceRequest.hpp"

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMLeaseComponent.hpp"
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMInterferenceRequestComponent.hpp"

namespace Game::Combat::Skill::FSM::Interference
{
	class SkillFSMInterferenceResolver
	{
	public:
		static void Update(Engine::ECS::EntityMgr& ecs);

	private:
		static const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest* 
			computeHighestPriorityRequest(
				Engine::ECS::EntityMgr& ecs,
				Engine::ECS::Entity e,
				Game::Combat::Skill::FSM::StateModel::SkillFSMInterferenceRequestComponent& reqs
			);

		static void acceptInterference(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity e,
			const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest& req,
			Game::Combat::Skill::FSM::StateModel::SkillFSMLeaseComponent& lease,
			float clock
		);

		static void updateInterference(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity e, 
			Game::Combat::Skill::FSM::StateModel::SkillFSMLeaseComponent& lease,
			float clock,
			float dt
		);
	};
}