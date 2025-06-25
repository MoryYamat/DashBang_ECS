// 

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::State::Action
{
	// キャラクターのスキル実行状態を更新する
	void UpdateCharacterSkillExecutionStateFromInstance
	(
		eNsECS::EntityMgr& ecs,
		float deltaTime
	);
}