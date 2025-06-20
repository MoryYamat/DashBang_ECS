#include "CharacterStateFeature.hpp"

// life state
#include "Game/Character/State/Init/InitCharaStateTransitionDatabase.hpp"
#include "Game/Character/State/System/Life/LifeStateSystem.hpp"

// movement state
#include "Game/Character/State/Init/InitCharaMoveStateTransitionDatabase.hpp"
#include "Game/Character/State/System/Movement/MovementStateSystem.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Feature::Character::StateFeature::InitTransitionDatabase(eNsECS::EntityMgr& ecs)
{
	// life
	gNsCharaLifeState::InitLifeStateTransitionDatabase(ecs);

	// movement
	gNsCharaMoveState::InitCharaMovementStateTransitionDatabase(ecs);
}

void Game::Feature::Character::StateFeature::UpdateCharacterState(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// Life
	gNsCharaLifeState::LifeStateSystem::UpdateStates(ecs);

	// movement
	gNsCharaMoveState::MovementStateSystem::UpdateStates(ecs, deltaTime);
}