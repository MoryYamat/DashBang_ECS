// 

#pragma once

#include "Engine/ECS/EntityManager.h"



namespace Game::Character::State::Action
{
	// キャラクターのスキル実行状態を更新する
	void UpdateCharacterSkillExecutionStateFromInstance
	(
		Engine::ECS::EntityMgr& ecs,
		float deltaTime
	);
}