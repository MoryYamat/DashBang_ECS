#include "MovementFSMFeature.hpp"

#include "Game/Character/FSM/Movement/MovementTransitionInitialize.hpp"

#include "Game/Character/FSM/Movement/MovementFSMSystem.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Feature::Character::FSM::MovementFSMFeature::InitializeMovementFSMDefinition(eNsECS::EntityMgr& ecs)
{
	gNsCharaFSMMovement::InitializeMovementTransitionSystem(ecs);
}

void Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementFSMSystem(eNsECS::EntityMgr& ecs)
{
	gNsCharaFSMMovement::UpdateMovementFSMSystem(ecs);
}