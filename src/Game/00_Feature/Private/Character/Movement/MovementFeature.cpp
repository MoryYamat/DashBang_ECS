#include "MovementFeature.h"

#include "Game/Character/Private/Control/Movement/System/CharacterMovementSystem.h"
#include "Game/Character/Private/Control/Movement/System/CharacterFacingSystem.h"

#include "Game/Character/Private/Control/Movement/System/UpdateCharacterVelocity.h"

#include "Game/Input/Private/Intent/PlayerMovementIntentMapper.h"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Game/Character/Private/Control/Public/ControlApi.hpp"

// Intent
void Game::Feature::Character::MovementFeature::UpdateMovementIntent(Engine::ECS::EntityMgr& ecs)
{
	// Game::Input::Intent::MovementIntentMappingSystem::UpdatePlayerMovementIntent(ecs);
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

//
//
namespace Game::Feature::Character
{
	void MovementFeature::UpdateMovementIntent(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Input::Intent::MovementIntentMappingSystem::UpdatePlayerMovementIntent(ctx);
	}

	void MovementFeature::UpdateVelocity(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		
	}

	void MovementFeature::UpdateLogicFacing(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Character::Control::UpdateCharacterFacingIntent(ctx);
	}

	void MovementFeature::UpdateLogicPosition(Engine::WorldSystem::Core::WorldCtx& ctx) 
	{
		Game::Character::Control::UpdateActorPosition(ctx);
	}
}