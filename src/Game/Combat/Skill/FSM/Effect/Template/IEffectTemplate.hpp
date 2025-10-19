#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"



namespace Game::Combat::Skill::FSM::Effect
{
	using namespace Game::Combat::Skill::Def;
	using namespace Game::Combat::Skill::FSM;

	// 最適化問題
	struct IEffectTemplate
	{
		virtual ~IEffectTemplate() = default;

		virtual void execute(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity caster,
			const SkillDef& def,
			const SkillFSMContext& ctx
		) const = 0;
	};
}