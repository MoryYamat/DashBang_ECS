#include "MovementTransitionInitialize.hpp"

#include "MovementTransitionTable.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Character::FSM::Movement::InitializeMovementTransitionSystem(eNsECS::EntityMgr& ecs)
{
	auto& tableRes = ecs.createResource<gNsCharaFSMMovement::MovementTransitionTableResource>();

	tableRes.transitions = gNsCharaFSMMovement::BuildMovementTransitionTable();
}