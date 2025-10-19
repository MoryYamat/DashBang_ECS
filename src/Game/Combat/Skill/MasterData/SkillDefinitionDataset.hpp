// スキル定義用データセット

/// 基本的にステートマシン形式で実行されるスキルを考える
// スキル実行は以下のように分けて考える
// 1. スキル実行フェーズ：キャラクター側のスキル実行の段階を表す
// 2. 当たり判定：スキルの判定を行う
// 3. スキル効果：スキルがヒットした際に発生する効果を表す

//
// SkillDefinition
//├── MetaInfo
//├── ExecutionModel
//│   ├── PhaseTiming
//│   ├── SyncPolicy / CancelPolicy
//│   └── MovementLock, FacingLock
//├── TargetingDef
//├── TriggerList[SkillExecutionEvent → SkillAction[]]
//│   └── ActionType + Payload(e.g.Hitbox, VFX, Buff)
//├── Damage, Buff, Debuff, SummonSpec, etc.
//├── ResourceCost, Cooldown, CastConditions
//├── Tags / UI / VFX / SFX
//
//

// SkillDefinition
// ├── Metadata            // ID, 名前, カテゴリ, クールダウンなど
// ├── ExecutionModel      // フェーズ設計, 移動制限, キャンセル, チャージなど
// ├── PhaseTable          // 各フェーズの長さ・条件・移行ポリシー
// ├── EventTable          // 各イベントの発火条件と内容（トリガー）
// │   └──[Phase x Event] = > ActionList
// │                     ├─ 条件(Condition)
// │                     ├─ 実行内容(ActionType)
// │                     └─ アクション用パラメータ（DataPayload）
// └── HitEffects          // ダメージ, CC, バフ, デバフの定義


#pragma once

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Game/Combat/Skill/MasterData/SkillTrajectoryData.h"



#include <variant>
#include <vector>
#include <optional>

namespace Game::Combat::Skill::MasterData
{
	// スキルのカテゴリを定義する列挙型
	enum class SkillCategory
	{
		Melee, // 近接攻撃スキル
		Ranged, // 遠距離攻撃スキル
		Projectile, // 弾道攻撃スキル
		AreaOfEffect, // 範囲攻撃スキル
		Summon, // 召喚スキル
		Buff, // バフスキル
		Debuff, // デバフスキル
	};

	// スキルの実行フェーズを定義する列挙型
	enum class SkillExecutionPhase
	{
		Casting,// スキルのキャスト段階
		Active, // スキルのアクティブ段階(攻撃など)
		Recovery,// スキルの硬直
		Completed, // スキルの完了段階(終了状態)
		Interrupted, // スキルの中断段階(スタンや死亡など)
		Canceled, // スキルキャンセル
	};

	// スキルの実行フェーズイベントを定義する列挙型
	enum class SkillExecutionEvent
	{
		OnEneterCasting, // スキルのキャスト開始
		OnExitCasting, // スキルのキャスト終了
		OnEnterActive, // スキルのアクティブ開始
		OnExitActive, // スキルのアクティブ終了
		OnEnterRecovery, // スキルのリカバリー開始
		OnExitRecovery, // スキルのリカバリー終了
		OnCompleted, // スキルの完了
		OnInterrupted, // スキルの中断
		OnCanceled, // スキルのキャンセル
	};

	struct SkillPhaseTiming
	{
		float castingDuration = 0.0f; // キャスト時間
		float activeDuration = 0.0f; // アクティブ時間
		float recoveryDuration = 0.0f; // リカバリー時間
	};


	// スキルの実行フェーズの変化をもとに何を起こすかを定義する列挙型
	enum class SkillActionType
	{
		None, // スキルアクションなし
		SpawnHitbox, // 攻撃判定を生成
		PlayeEffect, // エフェクトを再生
		ApplyBuff, // バフを適用
		ApplyDebuff, // デバフを適用
	};

}

namespace Game::Combat::Skill::MasterData
{
	// Hit box Data

	struct SkillHitBoxDef
	{
		Game::Combat::Skill::Component::Attack2DShape shape; // 攻撃判定の形状
		Game::Combat::Skill::Data::TrajectoryType trajectoryType = Game::Combat::Skill::Data::TrajectoryType::None; // 軌跡タイプ
		Game::Combat::Skill::Data::SkillTrajectory::TrajectoryParamsVariant trajectoryParamas = Game::Combat::Skill::Data::SkillTrajectory::StaticTrajectory{};
	};

	struct SkillHitBoxData
	{
		SkillHitBoxDef hitBoxdef; // 攻撃判定の定義
	};
}

namespace Game::Combat::Skill::MasterData
{
	// スキルの遷移条件を定義する列挙型
	enum class SkillConditionType
	{
		ElapsedTime, // 経過時間
		Count, // 回数
		Phase, // フェーズ
		ExecutionEventOccured, // イベント発生
		Flag, // フラグ
		CollisionOccured, // 衝突発生
		StatThreshold, // ステータス閾値
		RandomChance, // ランダム確率
	};

	// 時間の条件
	struct ElapsedTimeCondition
	{
		float thresholdSeconds = 0.0f; // 経過時間の閾値
	};

	// 回数の条件
	struct CountCondition
	{
		uint8_t thresholdCount = 0; // 条件を満たす回数の閾値
	};

	// フェーズの条件
	struct PhaseCondition
	{
		SkillExecutionPhase requiredPhase;// 必要なフェーズ
	};


}

namespace Game::Combat::Skill::MasterData
{
	struct SkillActionEntry
	{
		SkillActionType type = SkillActionType::None; // アクションのタイプ

		// 実行条件 (後で追加)

		// アクションに必要なデータ
		std::variant<
			SkillHitBoxData // 攻撃判定の定義
		> data;
	};

	// 
	struct SkillEventTriggerDef
	{
		SkillExecutionEvent triggerEvent = SkillExecutionEvent::OnEneterCasting;// このイベントが発火するタイミング
		std::vector<SkillActionEntry> actions = {};// このイベントが発火したときに実行されるアクションのリスト
	};
}

namespace Game::Combat::Skill::MasterData
{
	struct SkillDefinition
	{
		// スキルのメタデータ
		int id = 0; // スキルID
		std::string name; // スキル名
		SkillCategory category = SkillCategory::Melee; // スキルのカテゴリ
		float cooldown = 1.0f; // スキルの再使用待機時間

		SkillEventTriggerDef eventTriggers; // イベントのトリガとアクションの定義

		// スキルのフェーズ設計
	};
}


// 設計
// 必要なデータ
// 1. スキルのメタデータ(ID, NAME, Category, Cooldown)
// 2. スキルの実行モデル