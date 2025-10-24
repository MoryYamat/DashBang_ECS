#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Private/Skill/Def/SkillDef.hpp"

#include "Game/Combat/Private/Skill/Component/SkillExecutionContextComponent.hpp"
#include "Game/Combat/Private/Skill/FSM/SkillStateTags.hpp"

#include "IResetHandler.hpp"

namespace Game::Combat::Skill::FSM::Reset
{

	struct ResetSkillExecutionContext : IResetHandler
	{
		void execute(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity caster,
			const Game::Combat::Skill::Def::SkillDef&,
			const Game::Combat::Skill::FSM::SkillFSMContext&
		)const override
		{
			if (ecs.hasComponent<Game::Combat::Skill::Component::SkillExecutionContextComponent>(caster))
			{
				auto& exec = ecs.get<Game::Combat::Skill::Component::SkillExecutionContextComponent>(caster);
				exec.elapsedTime = 0.0f;
				exec.phaseElapsedTime = 0.0f;
				exec.isInterrupted = false;
				exec.previousState = StateTag::NONE;
				exec.skillId = 0;
			}
		}
	};
}