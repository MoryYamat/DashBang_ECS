// キャラクターの論理状態定義とComponent (表現状態とは別)

#pragma once

#include "Game/Character/State/Behavior/CharacterState.h"

#include <optional>

// 最新版：状態の重なり(歩きながらアクションなど)を許すことを考えるためにComponentを直交分割した 
namespace Game::Character::State
{
	// 原則的に
	// 同じレイヤーには排他的な状態が定義され
	// 別のレイヤーには独立の状態が定義される

	// (他レイヤーの前提)最上位状態レイヤ
	enum class LifeState 
	{ 
		Alive,// 生存
		Dead // 死亡
	};
	struct CharacterLifeStateComponent
	{
		LifeState current = LifeState::Alive;
	};

	// 空間的移動状態
	enum class MovementState 
	{ 
		Idle,// アイドル 
		Moving, // 移動中
	};
	struct CharacterMovementStateComponent
	{
		MovementState current = MovementState::Idle;
	};

	// アクション的な意思表出
	enum class ActionState 
	{
		None, 
		SkillCasting,// 複数フェーズあるが上層では一括で扱う
		Rolling,// 回避スキルとして扱う
		Guarding,// ガード
		UsingItem // アイテム使用
	};
	struct CharacterActionStateComponent
	{
		ActionState current = ActionState::None;
	};

	// 状態異常などの受動的妨害
	enum class CCState 
	{ 
		None, 
		Stunned, // スタン
		KnockedBack // ノックバック
	};
	struct CharacterCrowdControlStateComponent
	{
		CCState current = CCState::None;
	};
}

// Stateを直交分割した後は廃止予定
namespace Game::Character::State
{
	struct CharacterStateComponent
	{
		CharacterBehaviorState current = CharacterBehaviorState::Idle;// 現在の状態
		CharacterBehaviorState previous = CharacterBehaviorState::Idle;// 

		CharacterCrowdControlState crowdControl = CharacterCrowdControlState::None;

		float timeInState = 0.0f; // 経過時間．遷移判断などに使
		bool canBeInterrupted = true; // 中断可否フラグ


		// 状態の遷移要求(オプション：システム間の非同期リクエスト用)
		// 状態の遷移要求(オプション：システム間の非同期リクエスト用)
		std::optional<CharacterBehaviorState> requestedNextState;

		// キャラクターが行動制限状態かどうか
		inline bool IsCrowdControlled(CharacterCrowdControlState s)
		{
			return s != CharacterCrowdControlState::None;
		}

		// キャラクターが行動可能状態かどうか
		inline bool CanPerformAction(const CharacterStateComponent& c)
		{
			return c.current != CharacterBehaviorState::Dead &&
				c.crowdControl == CharacterCrowdControlState::None;
		}
	};
}