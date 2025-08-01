#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::FSM::Movement
{
	void InitMovementFSMDatabase(eNsECS::EntityMgr& ecs);

	// MovementFSMの定義を初期化し，グローバルリソースに登録する関数
	// void InitializeMovementTransitionSystem(eNsECS::EntityMgr& ecs);
}