// 状態 CharacterStateComponent に基づいてキャラクターの移動・向き更新を行うシステム

#pragma once

#include "Engine/ECS/EntityManager.h"


// character stateに基づいてキャラクターの移動／向き更新
namespace Game::Character::Movement
{

	void CharacterMovementSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);
	//void UpdateMovement(ECS& ecs, float deltaTime);
}