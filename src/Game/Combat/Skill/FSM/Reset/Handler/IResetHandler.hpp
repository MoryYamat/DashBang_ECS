#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"


namespace Game::Combat::Skill::FSM::Reset
{
	using namespace Engine::ECS;

	using namespace Game::Combat::Skill::Def;

	struct IResetHandler
	{
		virtual ~IResetHandler() = default;

		virtual void execute(
			EntityMgr& ecs,
			Entity caster,
			const SkillDef& def,
			const SkillFSMContext& ctx
		)const = 0;
	};
}