#include "StateLayerFeature.hpp"

#include "Game/00_Feature/Character/State/CharacterStateFeature.hpp"

#include "Game/Character/State/System/Action/Skill/CharacterSkillExecutionStateUpdater.hpp"

#include "Game/00_Feature/Character/FSM/Movement/MovementFSMFeature.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Layer::StateLayerFeature::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// Update Character Skill Execution State
	// gNsFeature::Character::StateFeature::UPdateCharacterSkillExecutionState(ecs, deltaTime);

	// Update Character State
	// gNsFeature::Character::StateFeature::UpdateCharacterState(ecs, deltaTime);

	// update Movement FSM
	gNsFeature::Character::FSM::MovementFSMFeature::UpdateMovementFSMSystem(ecs);
}