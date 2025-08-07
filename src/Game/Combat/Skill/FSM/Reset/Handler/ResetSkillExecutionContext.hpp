#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"

#include "Game/Combat/Skill/Component/SkillExecutionContextComponent.hpp"
#include "Game/Combat/Skill/FSM/SkillStateTags.hpp"

#include "IResetHandler.hpp"

namespace Game::Combat::Skill::FSM::Reset
{
	using namespace Engine::ECS;
	using namespace Game::Combat::Skill::Def;
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::FSM;

	struct ResetSkillExecutionContext : IResetHandler
	{
		void execute(
			EntityMgr& ecs,
			Entity caster,
			const SkillDef&,
			const SkillFSMContext&
		)const override
		{
			if (ecs.hasComponent<SkillExecutionContextComponent>(caster))
			{
				auto& exec = ecs.get<SkillExecutionContextComponent>(caster);
				exec.elapsedTime = 0.0f;
				exec.phaseElapsedTime = 0.0f;
				exec.isInterrupted = false;
				exec.previousState = StateTag::NONE;
				exec.skillId = 0;
			}
		}
	};
}