#include "StateLayerFeature.hpp"

#include "Game/00_Feature/Character/State/CharacterStateFeature.hpp"

#include "Game/Character/State/System/Action/Skill/CharacterSkillExecutionStateUpdater.hpp"

#include "Game/00_Feature/Character/FSM/Movement/MovementFSMFeature.hpp"
#include "Game/00_Feature/Character/FSM/Skill/SkillFSMFeature.hpp"
#include "Game/00_Feature/Character/FSM/CC/CCFSMFeature.hpp"


#include "Common/GameNamespaceDecl.h"

void Game::Layer::StateLayerFeature::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// Update Character Skill Execution State
	// gNsFeature::Character::StateFeature::UPdateCharacterSkillExecutionState(ecs, deltaTime);

	// Update Character State
	// gNsFeature::Character::StateFeature::UpdateCharacterState(ecs, deltaTime);

	// Create FSM Transition Requests
	// update Movement FSM
	gNsFeature::Character::FSM::MovementFSMFeature::UpdateMovementFSMSystem(ecs);

	// update Skill FSM
	Game::Feature::Character::Skill::FSM::SkillFSMFeature::UpdateSkillFSMSystem(ecs, deltaTime);

	// update CC FSM
	gNsFeature::Character::FSM::CCFSMFeature::UpdateCCFSMSystem(ecs);


	// =========== Resolver ==========

	Game::Feature::Character::Skill::FSM::SkillFSMFeature::UpdateSkillFSMResolverSystem(ecs, deltaTime);
	gNsFeature::Character::FSM::MovementFSMFeature::UpdateMovementFSMResolverSystem(ecs, deltaTime);

	gNsFeature::Character::FSM::CCFSMFeature::UpdateCCFSMResolverSystem(ecs, deltaTime);

	// ===== interference ===== 
	gNsFeature::Character::FSM::MovementFSMFeature::UpdateMovementFSMInterferenceResolver(ecs);
	Game::Feature::Character::Skill::FSM::SkillFSMFeature::UpdateSkillInterferenceResolver(ecs);


	// =========== FSM Scoped System =========

	gNsFeature::Character::FSM::MovementFSMFeature::UpdateMovementFSMScopedSystem(ecs, deltaTime);
}