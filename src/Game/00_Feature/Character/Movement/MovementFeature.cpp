#include "MovementFeature.h"

#include "Game/Character/Control/Movement/System/CharacterMovementSystem.h"
#include "Game/Character/Control/Movement/System/CharacterFacingSystem.h"

#include "Game/Character/Control/Movement/System/UpdateCharacterVelocity.h"

#include "Game/Input/Intent/PlayerMovementIntentMapper.h"


// Intent
void Game::Feature::Character::MovementFeature::UpdateMovementIntent(Engine::ECS::EntityMgr& ecs)
{
	Game::Input::Intent::MovementIntentMappingSystem::UpdatePlayerMovementIntent(ecs);
}

// 実行表現 (Action expression)
void Game::Feature::Character::MovementFeature::UpdateVelocity(Engine::ECS::EntityMgr& ecs)
{
	Game::Character::Movement::UpdateCharacterVelocity(ecs);
}


// logic
void Game::Feature::Character::MovementFeature::UpdateLogicFacing(Engine::ECS::EntityMgr& ecs)
{
	Game::Character::Movement::UpdatePlayerFacing(ecs);
}

// logic
void Game::Feature::Character::MovementFeature::UpdateLogicPosition(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	Game::Character::Movement::CharacterMovementSystem(ecs, deltaTime);
}