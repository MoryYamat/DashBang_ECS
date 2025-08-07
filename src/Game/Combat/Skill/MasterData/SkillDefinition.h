// skill definition data structure

#pragma once
#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Game/Combat/Skill/MasterData/SkillTrajectoryData.h"

#include "Game/Combat/Skill/MasterData/SkillCancelPhase.hpp" // SkillCancelBehavior

#include "Game/Combat/Skill/Component/SkillExecutionContextComponent.hpp"

#include "Game/Collision/Data/CollisionLayer.h"

#include <string>

#include <vector>

#include <cstdint>

#include <optional>

#include "Common/GameNamespaceDecl.h"

namespace Game::Combat::Skill::Data
{
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

	enum class SkillActionType
	{
		None, // スキルアクションなし
		SpawnHitbox, // 攻撃判定を生成
	};
}

namespace Game::Combat::Skill::Data
{
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
}

namespace Game::Combat::Skill::Data
{

	// ------------------------フェーズタイミング定義-------------------------
	// スキルのフェーズタイミングを定義する構造体
	struct SkillPhaseTiming
	{
		float castTime = 0.3f; // キャストタイム(発生までの時間)
		float recoveryTime = 0.4f; // リカバリータイム(スキル終了後の待機時間)
		float duration = 1.0f; // キャラが技を繰り出している時間(モーションの長さなど)
	};


	// ------------------------実行モデル・移動・キャンセル-------------------------
// 攻撃判定のキャラクター状態との同期／非同期
	enum class SkillCastSyncPolicy// 攻撃判定の制御主体をフェーズにゆだねるかどうか
	{
		None, // 同期なし
		Synchronous, // キャラクターの状態と同期
		Asynchronous // キャラクターの状態と非同期
	};

	// スキルの移動ロックポリシーを定義する列挙型
	enum class SkillMovementLockPolicy
	{
		Free,        // 完全に自由に移動できる
		Locked,      // 完全に移動不可（その場に固定）
		RootMotion,  // アニメーションで位置移動（物理的には移動しない）※将来
		Slide        // 方向は固定だが滑るように移動（突進系など）
	};

	// スキルのチャージ情報を定義する構造体
	struct SkillChargeSpecDef
	{
		bool isChargeSkill = false; // チャージ式スキルかどうか
		float chargeTime = 0.0f; // チャージ最大時間（チャージ式スキルの場合）
	};

	// スキルの実行モデルを定義する構造体
	struct SkillExecutionModelDef
	{
		SkillPhaseTiming timing; // フェーズタイミング情報
		SkillCastSyncPolicy castSyncPolicy; // キャスト状態と攻撃判定の同期／非同期ポリシー
		SkillCancelPhase cancelMask = SkillCancelPhase::None; // キャンセル可能なフェーズのマスク
		std::vector<int> cancelableSkillIds; // このスキル中にキャンセルして発動可能なスキルID
		SkillMovementLockPolicy movementLock = SkillMovementLockPolicy::Free; // スキルの移動ロックポリシー
		bool lockFacingDirection = false; // 詠唱中の向き固定
		SkillChargeSpecDef charge;
	};

}

// 
namespace Game::Combat::Skill::Data
{
	// ------------------------攻撃仕様-------------------------


	// スキルの攻撃判定ライフタイムの管理方法を定義する列挙型
	enum class AttackLifeTimeMode
	{
		SyncWithSkillPhase, // (= duration )meleeなど
		IndependentEntityLifetime, // Projectile
		AttachedToChildEntity// 設置型など
	};
	
	enum class SkillDespawnCondition : uint32_t
	{
		TimeElapsed = 1 << 0, // 時間経過によって消滅
		HitTarget = 1 << 1, // 何かに命中したら消滅
		HitObstacle = 1 << 2, // 障害物に当たったら消滅
		Manual = 1 << 3, // 外部操作で消滅（例：召喚主の死亡、ボタン解除など）

		DefaultProjectile = TimeElapsed | HitTarget | HitObstacle,
	};

	inline SkillDespawnCondition operator|(SkillDespawnCondition a, SkillDespawnCondition b)
	{
		return static_cast<SkillDespawnCondition>(
			static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}

	inline SkillDespawnCondition operator&(SkillDespawnCondition a, SkillDespawnCondition b)
	{
		return static_cast<SkillDespawnCondition>(
			static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
	}

	inline bool HasDespawnCondition(SkillDespawnCondition flags, SkillDespawnCondition check)
	{
		return static_cast<uint32_t>(flags & check) != 0;
	}


	// スキルのライフタイム仕様を定義する構造体
	struct SkillLifetimeSpecDef
	{
		AttackLifeTimeMode hitBoxlifetimePolicy = AttackLifeTimeMode::SyncWithSkillPhase;
		std::optional<float> duration = std::nullopt; // 必要なら明示
		SkillDespawnCondition despawnFlags = SkillDespawnCondition::TimeElapsed; // デフォルトは時間
	};

	// スキルの攻撃仕様を定義する構造体
	struct SkillAttackSpecDef
	{
		gNsSkillComp::SkillExecutionEvent triggerTiming = gNsSkillComp::SkillExecutionEvent::OnEnterActive; // スキルのトリガータイミング

		SkillLifetimeSpecDef lifetime; // ライフタイム仕様

		gNsSkillComp::Attack2DShape shape; // 形状定義
		TrajectoryType trajectoryType = gNsSkillData::TrajectoryType::None; // 軌跡タイプ
		SkillTrajectory::TrajectoryParamsVariant trajectoryParams = gNsSkillData::SkillTrajectory::StaticTrajectory{}; // 軌跡パラメータ
	};


}

namespace Game::Combat::Skill::Data
{
	struct SkillCollisionMaskDef
	{
		Game::Collision::Data::Layer selfLayer = Game::Collision::Data::Layer::Skill; // スキル自身のレイヤー
		uint32_t collidesWithMask = static_cast<uint32_t>(Game::Collision::Data::Layer::Enemy | Game::Collision::Data::Layer::Neutral); // 衝突するレイヤーマスク（デフォルトは敵のみ）
	};

	// 当たった相手に効果を与える条件(関係性ベース)
	enum class EffectTargetFilter
	{
		EnemyOnly,// 敵のみ
		AllyOnly,// 味方のみ
		SelfOnly,// 自分自身のみ
		All,// 全ての対象
		AllExceptSelf // 自分以外の全て
	};

	// スキルの効果対象を定義する構造体
	struct SkillEffectTargetDef
	{
		EffectTargetFilter targetFilter = EffectTargetFilter::EnemyOnly; // 効果を与える対象のフィルター
		SkillCollisionMaskDef collisionMask; // 衝突マスク定義
	};
}

namespace Game::Combat::Skill::Data
{
	// -------------------------スキル使用状態効果(ガード・アーマー)-------------------------

	// スーパーアーマーの種類
	enum class SkillSuperArmorType
	{
		None, // スーパーアーマーなし
		Invincible, // 無敵状態
		Unbreakable, // ブレイク不可なスーパーアーマー
		Breakable // ブレイク可能なスーパーアーマー
	};

	// ガードの有無
	enum class SkillGuardType
	{
		None, // ガードなし
		ForwardGuard, // 前方ガード
		BackwardGuard // 後方ガード
	};

	// スキルのアーマー情報を定義する構造体
	struct SkillDefenseDef
	{
		SkillSuperArmorType superArmorType = SkillSuperArmorType::None; // スーパーアーマーの種類
		SkillGuardType guardType = SkillGuardType::None; // ガードの有無
	};

}

namespace Game::Combat::Skill::Data
{


}

namespace Game::Combat::Skill::Data
{
	// -------------------------ヒット効果(即時(ダメージ・スタンなど))-------------------------
	// ヒット時の効果
	enum class OnHitEffectType
	{
		None, // 効果なし
		Stun, // スタン
		Knockback, // ノックバック
		Knockdown, // ノックダウン
	};

	// -------------------------ダメージ-------------------------
	enum class SkillDamageType
	{
		None, // ダメージなし
		Physical, // 物理ダメージ
		Magical, // 魔法ダメージ
		Pure // ピュアダメージ（防御無視）
	};

	struct SkillDamageDef
	{
		OnHitEffectType onHitEffect = OnHitEffectType::None; // ヒット時の効果（スタン、ノックバックなど）
		SkillDamageType damageType = SkillDamageType::None; // ダメージタイプ（物理、魔法、ピュアなど）
		float baseDamage = 0.0f; // 基本ダメージ
		float scalingFactor = 1.0f; // スケーリング係数（レベルやステータスに応じて変化する）
		bool isPercentBased = false; // パーセントベースのダメージかどうか（例：HPの割合ダメージなど）
	};

}

namespace Game::Combat::Skill::Data
{
	// スキルヒット継続効果

	enum class EffectApplicationTarget
	{
		Caster,// スキルを使用したキャスター自身
		Target,// スキルの対象（敵や味方）
		AreaOfEffect, // 範囲内の全てのエンティティ
		AreaAroundHitPoint,	// ヒットポイント周辺のエンティティ（範囲攻撃など）
	};

	// -------------------------バフ系-------------------------
	enum class SkillStatusBuffType
	{
		None, // バフなし
		AttackUp, // 攻撃力アップ
		DefenseUp, // 防御力アップ
		ManaRegen, // マナ回復速度アップ
		StaminaRegen, // スタミナ回復速度アップ
		CriticalHitChanceUp, // クリティカルヒット率アップ
		MovementSpeedUp, // 移動速度アップ
	};

	// バフ系のステータス変更のターゲットを定義する列挙型
	enum class StatusEffectTargetStat
	{
		None, // 対象なし
		Health, // ヘルス
		Mana, // マナ
		Stamina, // スタミナ
		AttackPower, // 攻撃力
		DefensePower, // 防御力
		CriticalHitChance, // クリティカルヒット率

		MovementSpeed, // 移動速度
	};

	// スキルのバフの仕様を定義する構造体
	struct BuffModifierDef
	{
		SkillStatusBuffType buffType = SkillStatusBuffType::None; // バフの種類（攻撃力アップ、防御力アップなど）
		StatusEffectTargetStat targetStat = StatusEffectTargetStat::None; // 対象のステータス
		float flatBonus = 0.0f; // フラットなボーナス（例：+10）
		float percentBonus = 0.0f; // パーセントベースのボーナス（例：+20%）
		bool isMultiplicative = false; // 乗算ボーナスかどうか（例：攻撃力アップが乗算されるかどうか）
		float duration = 10.0f; // バフの持続時間（秒）
	};

	struct BuffEffectDef
	{
		std::string buffName; // バフの名前
		std::vector<BuffModifierDef> modifiers; // バフのステータス変更のリスト
		EffectApplicationTarget applyTo = EffectApplicationTarget::Caster; // 効果を適用する対象（キャスター、ターゲット、範囲など）
	};



	// -------------------------デバフ系-------------------------

	// 付与する状態異常の種類
	enum class SkillStatusDebuffType
	{
		None, // 状態異常なし
		Poison, // 毒
		Burn, // 火傷
		Frost, // 凍結
		Silence, // 沈黙
		Slow, // スロー
		DefenseDown, // 防御ダウン
	};

	// デバフ系のステータス変更のターゲットを定義する列挙型
	struct DebuffModifierDef
	{
		SkillStatusDebuffType debuffType = SkillStatusDebuffType::None; // デバフの種類（毒、火傷、凍結など）
		StatusEffectTargetStat targetStat = StatusEffectTargetStat::None; // 対象のステータス
		float flatPenalty = 0.0f; // フラットなペナルティ（例：-10）
		float percentPenalty = 0.0f; // パーセントベースのペナルティ（例：-20%）
		bool isMultiplicative = false; // 乗算ペナルティかどうか（例：防御力ダウンが乗算されるかどうか）
		float duration = 10.0f; // デバフの持続時間（秒）
	};

	// スキルのデバフの仕様を定義する構造体
	struct DebuffEffectDef
	{
		std::string debuffName; // デバフの名前
		std::vector<DebuffModifierDef> modifiers; // デバフのステータス変更のリスト
		EffectApplicationTarget applyTo = EffectApplicationTarget::Caster; // 効果を適用する対象（キャスター、ターゲット、範囲など）
	};
}


namespace Game::Combat::Skill::Data
{

	// -------------------------リソース-------------------------

	// スキルの使用コストを定義する構造体
	struct SkillCostDef
	{
		float manaCost = 0.0f; // マナコスト
		float staminaCost = 0.0f; // スタミナコスト
		float healthCost = 0.0f; // ヘルスコスト
	};
}

namespace Game::Combat::Skill::Data
{
	// Fixme: 召喚物は特殊な処理フローになると思われるので，再考の余地あり．スキル定義が大規模になる
	enum class SummonFollowType
	{
		Fixed, // 固定位置に留まる
		Follow, // プレイヤーを追従する
		Guard, // 指定位置をガードする
		Patrol // 指定パトロールルートを巡回する
	};

	struct SkillSummonSpecDef
	{
		std::string summonEntityPrototypeId; // 召喚するエンティティのプロトタイプID
		float summonDuration = 10.0f; // 召喚物の持続時間（秒）
		SummonFollowType followType = SummonFollowType::Fixed; // 召喚物のフォロータイプ
	};

}

// スキル情報定義
namespace Game::Combat::Skill::Data
{
	// スキル定義構造体
	struct SkillDefinition
	{
		int id = 0; // 一意のID
		std::string name; // 名前
		SkillCategory category = SkillCategory::Melee; // スキルのカテゴリ

		SkillExecutionModelDef execution; // スキルの実行モデル

		// スキル攻撃仕様(スキル形状・軌跡)
		SkillAttackSpecDef attackSpec; // スキルの攻撃仕様

		// スキルの防御仕様(スーパーアーマー・ガード)
		SkillDefenseDef defense; // スキルの防御仕様

		// 誰に効果を与えるか(衝突マスク・対象フィルター)（攻撃：敵，バフ：味方）
		SkillEffectTargetDef effectTarget; // スキルの効果対象定義

		// リソースコスト
		SkillCostDef cost; // スキルの使用コスト

		// ヒット時即時効果(ダメージ・CC)
		SkillDamageDef damage; // スキルのダメージ情報

		// バフ・デバフ
		BuffEffectDef buffEffect; // スキルのバフ効果
		DebuffEffectDef debuffEffect; // スキルのデバフ効果

		// std::optional<SkillSummonSpecDef> summonSpec;

		float cooldown = 1.0f; // スキルの再使用待機時間
	};

	//// スキル情報定義
	//struct SkillDefinition
	//{
	//	int id = 0;// 一意のID
	//	std::string name;// 名前

	//	float castTime = 0.3f;// キャストタイム(発生までの時間)
	//	float recoveryTime = 0.4f;// スキル
	//	float duration = 1.0f;// スキル本体の持続時間	
	//	float cooldown = 1.0f;// スキルの再使用待機時間

	//	// 廃棄予定のフラグ
	//	bool canBeInterruptedDuringCast = true;
	//	bool canBeInterruptedDuringActive = false;
	//	bool canBeInterruptedDuringRecovery = true;

	//	gNsSkillComp::Attack2DShape shape;// 形状定義

	//	// 軌跡データ
	//	TrajectoryType trajectoryType = gNsSkillData::TrajectoryType::None;// 軌跡タイプ
	//	SkillTrajectory::TrajectoryParamsVariant trajectoryParams = gNsSkillData::SkillTrajectory::StaticTrajectory{};// 軌跡パラメータ

	//	// スキルの途中キャンセル可否
	//	SkillCancelPhase cancelBehavior = SkillCancelPhase::None; // キャンセル可能なフェーズ

	//	std::vector<int> cancelableSkillIds = {}; // このスキル中にキャンセルして発動可能なスキルID

	//	// スキルのキャスト状態と攻撃判定の同期／非同期
	//	SkillCastSyncType castSyncType = SkillCastSyncType::Synchronous;

	//	// スーパーアーマーの種類
	//	SkillSuperArmorType superArmorType = SkillSuperArmorType::None;

	//	// ガードの有無
	//	SkillGuardType guardType = SkillGuardType::None;

	//	// ヒット時の効果
	//	OnHitEffectType onHitEffect = OnHitEffectType::None;

	//	// 付与する状態異常の種類
	//	SkillStatusEffectType statusEffectType = SkillStatusEffectType::None;

	//	// スキルコスト
	//	float manaCost = 0.0f; // マナコスト
	//	float staminaCost = 0.0f; // スタミナコスト
	//	float healthCost = 0.0f; // ヘルスコスト

	//	// 詠唱中の向き固定
	//	bool lockFacingDirection = false; // 詠唱中に向きを固定するかどうか

	//	// チャージ式スキル
	//	bool isChargeSkill = false; // チャージ式スキルかどうか
	//	float chargeTime = 0.0f; // チャージ時間（チャージ式スキルの場合）
	//};
}

