// 状態遷移ルールを定義する
#pragma once 

#include "Game/Character/State/Behavior/CharacterState.h"

#include "Game/Character/State/Component/CharacterStateComponent.h"

#include "Common/GameNamespaceDecl.h"

#include <vector>
#include <string>

// 最新版：状態の重なり(歩きながらアクションなど)を許すことを考えるためにComponentを直交分割した 
namespace Game::Character::State
{

	// 遷移条件の種類
	enum class TransitionConditionType
	{
		CanBeInterrupted,// 中断
		NotCrowdControlled,// CC
		HasTimeElapsed,// 一定時間経過
		// 今後 HPやターゲットの有無なども追加可能
	};

	// 条件パラメータ
	struct TransitionCondition
	{
		TransitionConditionType type;

		// 数値パラメータ (必要時間，閾値など)
		float floatValue = 0.0f;

		// 拡張用
		std::string customData;
	};

	struct LifeStateTransitionRule
	{
		LifeState from;
		LifeState to;
		std::vector<TransitionCondition> conditions;
	};

	struct MovementStateTransitionRule
	{
		MovementState from;
		MovementState to;
		std::vector<TransitionCondition> conditions;
	};

	struct ActionStateTransitionRule
	{
		ActionState from;
		ActionState to;
		std::vector<TransitionCondition> conditions;
	};
}

// Stateを直交分割した後は廃止予定
namespace Game::Character::State
{

	// 1つの状態遷移ルール(例: Idle -> Moving)
	struct StateTransitionRule
	{
		gNsCharacterState::CharacterBehaviorState from;
		gNsCharacterState::CharacterBehaviorState to;

		// この遷移に必要な条件リスト (すべて満たす必要がある)
		std::vector<TransitionCondition> conditions;
	};
}