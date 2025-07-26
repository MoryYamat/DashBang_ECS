#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Skill/Def/SkillDef.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/FSM/Effect/Hook/SkillEffectHook.hpp"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Combat::Skill::FSM::System
{
	using namespace Game::Combat::Skill::FSM::Effect;
	using namespace Game::Combat::Skill::Def;

	class SkillFSMResolverSystem
	{
	public:
		static void Update(eNsECS::EntityMgr& ecs, float deltaTime);

		static void tryTriggerEffect(
			const SkillEffectHook& hook,
			eNsECS::EntityMgr& ecs,
			eNsECS::Entity eExec,
			eNsECS::Entity caster,
			const SkillDef& def,
			const SkillFSMContext& ctx,
			std::type_index current,
			std::type_index previous
		);
	};
}