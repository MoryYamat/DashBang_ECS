#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"
#include "Game/Combat/Skill/FSM/SkillStateTags.hpp"

#include "Game/Combat/Skill/Component/SkillExecutionComponent.hpp"

#include "Common/EngineNamespaceDecl.h"

#include "IEffectTemplate.hpp"

namespace Game::Combat::Skill::FSM::Effect
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::FSM;

	// SkillExecutionèÓïÒÇÉäÉZÉbÉg
	struct ResetExecutionStateEffect :IEffectTemplate
	{
		void execute(
			eNsECS::EntityMgr& ecs,
			eNsECS::Entity caster,
			const SkillDef& def,
			const SkillFSMContext& ctx
		) const override
		{
			if (!ecs.hasComponent<SkillExecutionComponent>(caster)) return;

			auto& exec = ecs.get<SkillExecutionComponent>(caster);
			exec.elapsedTime = 0.0f;
			exec.phaseElapsedTime = 0.0f;
			exec.isInterrupted = false;
			exec.previousState = StateTag::NONE;
			exec.skillId = 0;
		}
	};
}