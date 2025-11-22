#include "StateLayerFeature.hpp"

#include "Game/00_Feature/Private/Character/State/CharacterStateFeature.hpp"

#include "Game/Character/Private/State/System/Action/Skill/CharacterSkillExecutionStateUpdater.hpp"

#include "Game/00_Feature/Private/Character/FSM/Movement/MovementFSMFeature.hpp"
#include "Game/00_Feature/Private/Character/FSM/Skill/SkillFSMFeature.hpp"
#include "Game/00_Feature/Private/Character/FSM/CC/CCFSMFeature.hpp"


#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Character/Private/FSM/Public/MovementAxisApi.hpp"

void Game::Layer::StateLayerFeature::Update(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	// Update Character Skill Execution State
	// Game::Feature::Character::StateFeature::UPdateCharacterSkillExecutionState(ecs, deltaTime);

	// Update Character State
	// Game::Feature::Character::StateFeature::UpdateCharacterState(ecs, deltaTime);

	// Create FSM Transition Requests
	// update Movement FSM
	// Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementFSMSystem(ecs);

	// update Skill FSM
	// Game::Feature::Character::Skill::FSM::SkillFSMFeature::UpdateSkillFSMSystem(ecs, deltaTime);

	// update CC FSM
	// Game::Feature::Character::FSM::CCFSMFeature::UpdateCCFSMSystem(ecs);


	// =========== Resolver ==========

	// Game::Feature::Character::Skill::FSM::SkillFSMFeature::UpdateSkillFSMResolverSystem(ecs, deltaTime);
	// Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementFSMResolverSystem(ecs, deltaTime);

	// Game::Feature::Character::FSM::CCFSMFeature::UpdateCCFSMResolverSystem(ecs, deltaTime);

	// ===== interference ===== 
	/// Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementFSMInterferenceResolver(ecs);
	// Game::Feature::Character::Skill::FSM::SkillFSMFeature::UpdateSkillInterferenceResolver(ecs);


	// =========== FSM Scoped System =========

	// Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementFSMScopedSystem(ecs, deltaTime);
}

void Game::Layer::StateLayerFeature::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	Game::Character::FSM::Movement::UpdateMovementAll(ctx);
}