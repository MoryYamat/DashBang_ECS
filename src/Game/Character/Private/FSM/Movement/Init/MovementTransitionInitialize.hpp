#pragma once

#include "Engine/ECS/EntityManager.h"


namespace Game::Character::FSM::Movement
{
	void InitMovementFSMDatabase(Engine::ECS::EntityMgr& ecs);

	// MovementFSMの定義を初期化し，グローバルリソースに登録する関数
	// void InitializeMovementTransitionSystem(Engine::ECS::EntityMgr& ecs);
}