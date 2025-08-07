#pragma once
#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"

#include "Game/Combat/Skill/Component/SkillEffectExecutionRecordComponent.hpp"

#include "IResetHandler.hpp"

namespace Game::Combat::Skill::FSM::Reset
{
	using namespace Engine::ECS;
	using namespace Game::Combat::Skill::Def;
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::FSM;

	struct ClearEffectExecutionLog : IResetHandler
	{
		void execute(
			EntityMgr& ecs,
			Entity caster,
			const SkillDef&,
			const SkillFSMContext&
		) const override
		{
			if (ecs.hasComponent<SkillEffectExecutionRecordComponent>(caster))
			{
				auto& record = ecs.get<SkillEffectExecutionRecordComponent>(caster);
				record.clear();
			}
		}
	};
}