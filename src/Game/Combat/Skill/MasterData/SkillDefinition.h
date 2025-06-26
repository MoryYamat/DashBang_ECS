// skill definition data structure

#pragma once
#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Game/Combat/Skill/MasterData/SkillTrajectoryData.h"

#include "Game/Combat/Skill/MasterData/SkillCancelPhase.hpp" // SkillCancelBehavior

#include <string>

#include <vector>

#include <cstdint>

#include "Common/GameNamespaceDecl.h"


namespace Game::Combat::Skill::Data
{
	// スキルのトリガータイミングを定義する列挙型
	enum class SkillTriggerTiming
	{
		OnCastingStart,// スキルのキャスト開始時
		OnCastingEnd,// スキルのキャスト終了時
		OnActiveStart,// スキルがアクティブになったとき
		OnActiveEnd,// スキルがアクティブを終了したとき
		OnRecoveryStart,// スキルのリカバリー開始時
	};

	// スキルの攻撃判定ライフタイムの管理方法を定義する列挙型
	enum class AttackLifeTimeMode
	{
		SyncWithSkillPhase, // meleeなど
		IndependentEntityLifetime, // Projectile
		AttachedToChildEntity// 設置型など
	};
}

// 
namespace Game::Combat::Skill::Data
{

	// 攻撃判定のキャラクター状態との同期／非同期
	enum class SkillCastSyncType
	{
		None, // 同期なし
		Synchronous, // キャラクターの状態と同期
		Asynchronous // キャラクターの状態と非同期
	};

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

	// ヒット時の効果
	enum class OnHitEffectType
	{
		None, // 効果なし
		Stun, // スタン
		Knockback, // ノックバック
		Knockdown, // ノックダウン
	};

	// 付与する状態異常の種類
	enum class SkillStatusEffectType
	{
		None, // 状態異常なし
		Poison, // 毒
		Burn, // 火傷
		Frost, // 凍結
		Silence, // 沈黙
	};

	enum class SkillMovementLockType
	{
		Free,        // 完全に自由に移動できる
		Locked,      // 完全に移動不可（その場に固定）
		RootMotion,  // アニメーションで位置移動（物理的には移動しない）※将来
		Slide        // 方向は固定だが滑るように移動（突進系など）
	};
}

// 構造体
namespace Game::Combat::Skill::Data
{
	// スキルの使用コストを定義する構造体
	struct SkillCost
	{
		float manaCost = 0.0f; // マナコスト
		float staminaCost = 0.0f; // スタミナコスト
		float healthCost = 0.0f; // ヘルスコスト
	};

	// スキルのフェーズタイミングを定義する構造体
	struct SkillPhaseTiming
	{
		float castTime = 0.3f; // キャストタイム(発生までの時間)
		float recoveryTime = 0.4f; // リカバリータイム(スキル終了後の待機時間)
		float duration = 1.0f; // スキル本体の持続時間
		float cooldown = 1.0f; // スキルの再使用待機時間
	};

	// スキルのアーマー情報を定義する構造体
	struct SkillArmorInfo
	{
		gNsSkillData::SkillSuperArmorType superArmorType = gNsSkillData::SkillSuperArmorType::None; // スーパーアーマーの種類
		gNsSkillData::SkillGuardType guardType = gNsSkillData::SkillGuardType::None; // ガードの有無
	};

	// スキルのヒット効果を定義する構造体
	struct SkillHitEffect
	{
		gNsSkillData::OnHitEffectType onHitEffect = gNsSkillData::OnHitEffectType::None; // ヒット時の効果
		gNsSkillData::SkillStatusEffectType statusEffectType = gNsSkillData::SkillStatusEffectType::None; // 付与する状態異常の種類
	};

	// スキルのチャージ情報を定義する構造体
	struct SkillChargeInfo
	{
		bool isChargeSkill = false; // チャージ式スキルかどうか
		float chargeTime = 0.0f; // チャージ時間（チャージ式スキルの場合）
	};

}

// スキル情報定義
namespace Game::Combat::Skill::Data
{
	struct SkillDefinition
	{
		int id = 0; // 一意のID
		std::string name; // 名前
		SkillPhaseTiming phaseTiming; // フェーズタイミング情報
		SkillCost cost; // スキルコスト
		SkillArmorInfo armorInfo; // アーマー情報
		SkillHitEffect hitEffect; // ヒット効果
		SkillChargeInfo chargeInfo; // チャージ情報

		SkillCancelPhase cancelBehavior = SkillCancelPhase::None; // スキルの途中キャンセル可否
		std::vector<int> cancelableSkillIds = {}; // このスキル中にキャンセルして発動可能なスキルID

		gNsSkillComp::Attack2DShape shape; // 形状定義
		TrajectoryType trajectoryType = gNsSkillData::TrajectoryType::None; // 軌跡タイプ
		SkillTrajectory::TrajectoryParamsVariant trajectoryParams = gNsSkillData::SkillTrajectory::StaticTrajectory{}; // 軌跡パラメータ

		SkillCastSyncType castSyncType = SkillCastSyncType::Synchronous; // スキルのキャスト状態と攻撃判定の同期／非同期
		SkillMovementLockType movementLockType = SkillMovementLockType::Free; // スキルの移動ロックタイプ

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

