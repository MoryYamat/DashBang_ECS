#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Combat::Skill::FSM::Effect
{
	using namespace Game::Combat::Skill::Def;
	using namespace Game::Combat::Skill::FSM;

	struct IEffectTemplate
	{
		virtual ~IEffectTemplate() = default;

		virtual void execute(
			eNsECS::EntityMgr& ecs,
			eNsECS::Entity caster,
			const SkillDef& def,
			const SkillFSMContext& ctx
		) const = 0;
	};
}