#include "CharacterStateFeature.hpp"

// life state
#include "Game/Character/State/Init/InitCharaStateTransitionDatabase.hpp"
#include "Game/Character/State/System/Life/LifeStateSystem.hpp"

// movement state
#include "Game/Character/State/Init/InitCharaMoveStateTransitionDatabase.hpp"
#include "Game/Character/State/System/Movement/MovementStateSystem.hpp"

// action state
#include "Game/Character/State/Init/InitCharaActionStateTransitionDatabase.hpp"
#include "Game/Character/State/System/Action/ActionStateSystem.hpp"

// skill execution state
#include "Game/Character/State/System/Action/Skill/CharacterSkillExecutionStateUpdater.hpp"

#include "Game/Character/State/System/Action/Skill/CharacterSkillExecutionResetSystem.hpp"



void Game::Feature::Character::StateFeature::InitTransitionDatabase(Engine::ECS::EntityMgr& ecs)
{
	// life
	Game::Character::State::Life::InitLifeStateTransitionDatabase(ecs);

	// movement
	Game::Character::State::Movement::InitCharaMovementStateTransitionDatabase(ecs);

	// action
	Game::Character::State::Action::InitCharaActionStateTransitionDatabase(ecs);
}

void Game::Feature::Character::StateFeature::UpdateCharacterState(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	// Life
	Game::Character::State::Life::LifeStateSystem::UpdateStates(ecs);

	// movement
	Game::Character::State::Movement::MovementStateSystem::UpdateStates(ecs, deltaTime);

	// action
	Game::Character::State::Action::ActionStateSystem::UpdateStates(ecs, deltaTime);
}

void Game::Feature::Character::StateFeature::UPdateCharacterSkillExecutionState(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	// キャラクターのスキル実行状態をリセットする (スキルを完了，もしくは発動していない場合)
	Game::Character::State::Action::CharacterSkillExecutionResetSystem::ResetSkillExecutionState(ecs, deltaTime);

	// キャラクターのスキル実行状態を更新する
	Game::Character::State::Action::UpdateCharacterSkillExecutionStateFromInstance(ecs, deltaTime);
}