#include "MovementFSMFeature.hpp"

#include "Game/Character/FSM/Movement/Init/MovementTransitionInitialize.hpp"

#include "Game/Character/Control/Movement/System/CharacterMovementIntentResolver.hpp"

#include "Game/Character/FSM/Movement/System/MovementFSMResolverSystem.hpp"
#include "Game/Character/FSM/Movement/System/MovementFSMSystem.hpp"

#include "Game/Character/FSM/Movement/System/MovementFSMScopedSystem.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Feature::Character::FSM::MovementFSMFeature::InitializeMovementFSMDefinition(eNsECS::EntityMgr& ecs)
{
	gNsCharaFSMMovement::InitMovementFSMDatabase(ecs);
	//gNsCharaFSMMovement::InitializeMovementTransitionSystem(ecs);
}

void Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementIntentResolver(eNsECS::EntityMgr& ecs)
{
	//Game::Character::Control::Movement::UpdateMovementIntentResolverSystem(ecs);// çÌèúó\íËÅFFSMì±ì¸å„îpé~
}

void Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementFSMSystem(eNsECS::EntityMgr& ecs)
{
	// state Scope
	gNsCharaFSMMovement::UpdateMovementFSMSystem(ecs);
}

void Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementFSMResolverSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	Game::Character::FSM::Movement::System::MovementFSMResolverSystem::Update(ecs, deltaTime);
}

void Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementFSMScopedSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// fsm scope
	Game::Character::FSM::Movement::System::MovementFSMScopedEffectSystem::Update(ecs, deltaTime);
}