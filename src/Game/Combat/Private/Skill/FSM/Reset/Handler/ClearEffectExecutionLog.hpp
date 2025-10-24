#pragma once
#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Private/Skill/Def/SkillDef.hpp"

#include "Game/Combat/Private/Skill/Component/SkillEffectExecutionRecordComponent.hpp"

#include "IResetHandler.hpp"

namespace Game::Combat::Skill::FSM::Reset
{


	struct ClearEffectExecutionLog : IResetHandler
	{
		void execute(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity caster,
			const Game::Combat::Skill::Def::SkillDef&,
			const Game::Combat::Skill::FSM::SkillFSMContext&
		) const override
		{
			using namespace Game::Combat::Skill::Component;

			if (ecs.hasComponent<SkillEffectExecutionRecordComponent>(caster))
			{
				auto& record = ecs.get<SkillEffectExecutionRecordComponent>(caster);
				record.clear();
			}
		}
	};
}