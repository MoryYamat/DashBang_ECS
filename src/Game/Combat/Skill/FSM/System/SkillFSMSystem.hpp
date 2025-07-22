#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Common/EngineNamespaceDecl.h"

#include "Game/Combat/Skill/Def/SkillDef.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/FSM/Effect/Hook/SkillEffectHook.hpp"

#include <typeindex>

namespace Game::Combat::Skill::FSM
{
	using namespace Game::Combat::Skill::FSM::Effect;
	using namespace Game::Combat::Skill::Def;

	void UpdateSkillFSMSystem(eNsECS::EntityMgr& ecs, float deltaTime);

	void tryTriggerEffect(
		const SkillEffectHook& hook,
		eNsECS::EntityMgr& ecs,
		eNsECS::Entity eExec,
		eNsECS::Entity caster,
		const SkillDef& def,
		const SkillFSMContext& ctx,
		std::type_index current,
		std::type_index previous
	);
}

