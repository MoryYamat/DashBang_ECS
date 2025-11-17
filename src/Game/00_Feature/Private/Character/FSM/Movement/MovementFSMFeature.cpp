#include "MovementFSMFeature.hpp"

#include "Game/Character/Private/FSM/Movement/Init/MovementTransitionInitialize.hpp"

#include "Game/Character/Private/Control/Movement/System/CharacterMovementIntentResolver.hpp"

#include "Game/Character/Private/FSM/Movement/System/MovementFSMResolverSystem.hpp"
#include "Game/Character/Private/FSM/Movement/System/MovementFSMSystem.hpp"

#include "Game/Character/Private/FSM/Movement/System/MovementFSMScopedSystem.hpp"

#include "Game/Character/Private/FSM/Movement/Interference/MovementFSMInterferenceResolver.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

// #include "Game/Character/Private/FSM/Public/MovementTypes.hpp"
// #include "Game/Character/Private/FSM/Public/MovementAxisApi.hpp"

void Game::Feature::Character::FSM::MovementFSMFeature::InitializeMovementFSMDefinition(Engine::ECS::EntityMgr& ecs)
{
	Game::Character::FSM::Movement::InitMovementFSMDatabase(ecs);
	//Game::Character::FSM::Movement::InitializeMovementTransitionSystem(ecs);
}

void Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementIntentResolver(Engine::ECS::EntityMgr& ecs)
{
	//Game::Character::Control::Movement::UpdateMovementIntentResolverSystem(ecs);// 削除予定：FSM導入後廃止
}

void Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementFSMSystem(Engine::ECS::EntityMgr& ecs)
{
	// state Scope
	Game::Character::FSM::Movement::UpdateMovementFSMSystem(ecs);
}

void Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementFSMResolverSystem(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	Game::Character::FSM::Movement::System::MovementFSMResolverSystem::Update(ecs, deltaTime);
}

void Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementFSMScopedSystem(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	// fsm scope
	Game::Character::FSM::Movement::System::MovementFSMScopedEffectSystem::Update(ecs, deltaTime);
}

namespace Game::Feature::Character::FSM
{
	void MovementFSMFeature::UpdateMovementFSMInterferenceResolver(Engine::ECS::EntityMgr& ecs)
	{
		Game::Character::FSM::Movement::Interference::MovementFSMInterferenceResolver::Update(ecs);
	}
}

namespace Game::Feature::Character::FSM
{
}