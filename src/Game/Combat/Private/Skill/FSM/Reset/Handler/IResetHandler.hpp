#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Private/Skill/Def/SkillDef.hpp"


namespace Game::Combat::Skill::FSM::Reset
{

	struct IResetHandler
	{
		virtual ~IResetHandler() = default;

		virtual void execute(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity caster,
			const Game::Combat::Skill::Def::SkillDef& def,
			const Game::Combat::Skill::FSM::SkillFSMContext& ctx
		)const = 0;
	};
}