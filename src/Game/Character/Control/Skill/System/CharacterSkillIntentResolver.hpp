#pragma once

#include "Game/Combat/Skill/MasterData/SkillEntry.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillStateComponent.hpp"

#include "Game/Combat/Skill/FSM/Trigger/ISkillTriggerCondition.hpp"

#include "Engine/ECS/EntityManager.h"	
#include "Common/EngineNamespaceDecl.h"

#include "Common/GameNamespaceDecl.h"

namespace Game::Character::Control::Skill
{
	// スキル発動意図を反映するかしないか判定するシステム
	void UpdateSkillResolverSystem(eNsECS::EntityMgr& ecs);

	// 
	bool canTriggerSkill
	(
		const gNsSkillFSM::SkillStateComponent& state, 
		const gNsSkillData::SkillEntry& entry
	);

	// 削除予定：キャラクターのスキルの意図を反映するかしないか判定するシステム（FSM実装後廃止）
	void UpdateCharacterSkillIntentResovlver(eNsECS::EntityMgr& ecs);
}