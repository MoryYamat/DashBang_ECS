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

#include "Common/GameNamespaceDecl.h"

void Game::Feature::Character::StateFeature::InitTransitionDatabase(eNsECS::EntityMgr& ecs)
{
	// life
	gNsCharaLifeState::InitLifeStateTransitionDatabase(ecs);

	// movement
	gNsCharaMoveState::InitCharaMovementStateTransitionDatabase(ecs);

	// action
	gNsCharaActionState::InitCharaActionStateTransitionDatabase(ecs);
}

void Game::Feature::Character::StateFeature::UpdateCharacterState(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// Life
	gNsCharaLifeState::LifeStateSystem::UpdateStates(ecs);

	// movement
	gNsCharaMoveState::MovementStateSystem::UpdateStates(ecs, deltaTime);

	// action
	gNsCharaActionState::ActionStateSystem::UpdateStates(ecs, deltaTime);
}

void Game::Feature::Character::StateFeature::UPdateCharacterSkillExecutionState(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// キャラクターのスキル実行状態をリセットする (スキルを完了，もしくは発動していない場合)
	gNsCharaActionState::CharacterSkillExecutionResetSystem::ResetSkillExecutionState(ecs, deltaTime);

	// キャラクターのスキル実行状態を更新する
	gNsCharaActionState::UpdateCharacterSkillExecutionStateFromInstance(ecs, deltaTime);
}