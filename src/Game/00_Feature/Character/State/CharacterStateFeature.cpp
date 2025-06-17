#include "CharacterStateFeature.hpp"

#include "Game/Character/State/System/CharacterStateSystem.h"
#include "Game/Character/State/Rule/Init/InitStateTransitionDatabase.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Feature::Character::StateFeature::InitTransitionDatabase(eNsECS::EntityMgr& ecs)
{
	gNsCharacterState::InitCharacterStateTransitionDatabase(ecs);
}

void Game::Feature::Character::StateFeature::UpdateCharacterState(eNsECS::EntityMgr& ecs, float deltaTime)
{
	gNsCharacterState::CharacterStateSystem::UpdateStates(ecs, deltaTime);
}