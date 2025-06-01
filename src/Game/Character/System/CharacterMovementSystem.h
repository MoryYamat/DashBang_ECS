// 状態 CharacterStateComponent に基づいてキャラクターの移動・向き更新を行うシステム

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

// character stateに基づいてキャラクターの移動／向き更新
namespace Game::Character::Movement
{

	void UpdateCharacterVelocity(eNsECS::EntityMgr& ecs);
	//void UpdateMovement(ECS& ecs, float deltaTime);
}