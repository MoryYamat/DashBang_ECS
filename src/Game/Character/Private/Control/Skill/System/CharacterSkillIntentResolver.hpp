#pragma once

#include "Game/Combat/Private/Skill/MasterData/SkillEntry.hpp"
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillStateComponent.hpp"

#include "Game/Combat/Private/Skill/FSM/Trigger/ISkillTriggerCondition.hpp"

#include "Engine/ECS/EntityManager.h"	

namespace Game::Character::Control::Skill
{
	// スキル発動意図を反映するかしないか判定するシステム
	void UpdateSkillResolverSystem(Engine::ECS::EntityMgr& ecs);

	// 
	bool canTriggerSkill
	(
		const Game::Combat::Skill::FSM::StateModel::SkillStateComponent& state,
		const Game::Combat::Skill::Data::SkillEntry& entry
	);


}