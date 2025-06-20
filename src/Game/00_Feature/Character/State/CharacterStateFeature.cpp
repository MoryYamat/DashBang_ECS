#include "CharacterStateFeature.hpp"

#include "Game/Character/State/Init/InitCharaStateTransitionDatabase.hpp"

#include "Game/Character/State/System/Life/LifeStateSystem.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Feature::Character::StateFeature::InitTransitionDatabase(eNsECS::EntityMgr& ecs)
{
	gNsCharaLifeState::InitLifeStateTransitionDatabase(ecs);
}

void Game::Feature::Character::StateFeature::UpdateCharacterState(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// Life
	gNsCharaLifeState::LifeStateSystem::UpdateStates(ecs);


}