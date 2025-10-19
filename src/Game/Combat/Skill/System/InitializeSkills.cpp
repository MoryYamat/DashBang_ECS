#include "InitializeSkills.h"




#include "Game/Combat/Skill/MasterData/SkillDatabase.h"
#include "Game/Combat/Skill/MasterData/SkillDefinition.h"
#include "Game/Combat/Skill/MasterData/SkillTrajectoryData.h"

#include "Game/Combat/Skill/MasterData/SkillDefinitionDataset.hpp"

#include <iostream>

//void Game::Combat::Skill::System::InitializeSkills(Engine::ECS::EntityMgr& ecs)
//{
//	using namespace Game::Combat::Skill::MasterData;
//
//	auto& db = ecs.createResource<Game::Combat::Skill::Data::SkillDatabase>();
//
//	SkillDefinition slash;
//}

void Game::Combat::Skill::System::InitializeSkills(Engine::ECS::EntityMgr& ecs)
{
	// Game::Combat::Skill::Data::SkillDatabase db;
	
	using namespace Game::Combat::Skill::Data;

	// ECSのリソースとしてスキルデータベースを作成
	auto& db = ecs.createResource<Game::Combat::Skill::Data::SkillDatabase>();

	

	SkillDefinition slash;
	slash.id = 1;
	slash.name = "Basic Slash";
	slash.category = SkillCategory::Melee; // スキルのカテゴリを設定

	// スキルの実行モデルを定義
	slash.execution = SkillExecutionModelDef{
		.timing = SkillPhaseTiming{
			.castTime = 0.3f, // キャストタイム(発生までの時間)
			.recoveryTime = 0.4f, // リカバリータイム(スキル終了後の待機時間)
			.duration = 1.0f // actionの持続時間
		},
		.castSyncPolicy = SkillCastSyncPolicy::Synchronous, // キャスト状態と攻撃判定の同期
		.cancelMask = SkillCancelPhase::All, // 全てのフェーズでキャンセル可能
		.cancelableSkillIds = { 2, 3 }, // このスキル中にキャンセルして発動可能なスキルID
		.movementLock = SkillMovementLockPolicy::Locked, // スキルの移動ロックタイプ：ロック
		.lockFacingDirection = true, // 詠唱中の向き固定
		.charge = SkillChargeSpecDef{
			.isChargeSkill = false, // チャージ式スキルかどうか
			.chargeTime = 0.0f // チャージ時間（チャージ式スキルの場合）
		},
	};

	// スキルの攻撃仕様を定義
	slash.attackSpec = SkillAttackSpecDef{
		.triggerTiming = Game::Combat::Skill::Component::SkillExecutionEvent::OnEnterActive, // スキルのトリガータイミング
		.lifetime = SkillLifetimeSpecDef{
			.hitBoxlifetimePolicy = AttackLifeTimeMode::SyncWithSkillPhase, // フェーズと同期
			.duration = 1.0f, // スキルの持続時間
			.despawnFlags = SkillDespawnCondition::TimeElapsed // 時間経過で消滅
		},
		.shape = Game::Combat::Skill::Component::Attack2DShape{ Game::Combat::Skill::Component::Sector2DAttack
		{
			.center = CanonicalDefaults::kLocalCenterXZ, // 中心位置
			.direction = CanonicalDefaults::kLocalForwardXZ,
			.angle = 0.5236f, // 約57度
			.radius = 10.0f
		} },
		.trajectoryType = TrajectoryType::None, // 軌跡タイプはなし
		.trajectoryParams = Game::Combat::Skill::Data::SkillTrajectory::StaticTrajectory{}, // 静的軌跡パラメータ
	};

	// スキルの防御仕様を定義
	slash.defense = SkillDefenseDef{
		.superArmorType = Game::Combat::Skill::Data::SkillSuperArmorType::None, // スーパーアーマーの種類
		.guardType = Game::Combat::Skill::Data::SkillGuardType::None // ガードの有無
	};


	// スキルのコストを定義
	slash.cost = SkillCostDef{
		.manaCost = 0.0f, // マナコスト
		.staminaCost = 0.0f, // スタミナコスト
		.healthCost = 0.0f // ヘルスコスト
	};

	// スキルのダメージ情報を定義
	slash.damage = SkillDamageDef{
		.onHitEffect = Game::Combat::Skill::Data::OnHitEffectType::None, // ヒット時の効果
		.damageType = SkillDamageType::Physical, // ダメージタイプ
		.baseDamage = 10.0f, // 基本ダメージ
		.isPercentBased = false, // パーセントベースのダメージかどうか
	};

	// バフ・デバフ：なし

	db.AddSkill(slash);

	// 投射物の基礎スキル定義
	SkillDefinition projectile;
	projectile.id = 2;
	projectile.name = "Projectile Skill";
	projectile.category = SkillCategory::Projectile; // スキルのカテゴリを設定

	// スキルの実行モデルを定義
	projectile.execution = SkillExecutionModelDef{
		.timing = SkillPhaseTiming{
			.castTime = 0.3f, // キャストタイム(発生までの時間)
			.recoveryTime = 0.3f, // リカバリータイム(スキル終了後の待機時間)
			.duration = 1.0f // actionの持続時間
		},
		.castSyncPolicy = SkillCastSyncPolicy::Asynchronous, // キャスト状態と攻撃判定の非同期
		.cancelMask = SkillCancelPhase::All, // 全てのフェーズでキャンセル可能
		.cancelableSkillIds = {}, // このスキル中にキャンセルして発動可能なスキルID
		.movementLock = SkillMovementLockPolicy::Free, // スキルの移動ロックタイプ：自由
		.lockFacingDirection = false, // 詠唱中の向き固定なし
		.charge = SkillChargeSpecDef{
			.isChargeSkill = false, // チャージ式スキルかどうか
			.chargeTime = 0.0f // チャージ時間（チャージ式スキルの場合）
		},
	};

	// スキルの攻撃仕様を定義
	projectile.attackSpec = SkillAttackSpecDef{
		.triggerTiming = Game::Combat::Skill::Component::SkillExecutionEvent::OnEnterActive, // スキルのトリガータイミング
		.lifetime = SkillLifetimeSpecDef{
			.hitBoxlifetimePolicy = AttackLifeTimeMode::IndependentEntityLifetime, // フェーズと同期
			.duration = 3.0f, // スキルの持続時間
			.despawnFlags = SkillDespawnCondition::DefaultProjectile // 時間経過で消滅
		},
		.shape = Game::Combat::Skill::Component::Attack2DShape{ Game::Combat::Skill::Component::Circle2DAttack
		{
			.center = CanonicalDefaults::kLocalCenterXZ, // 中心位置
			.radius = 5.0f // 半径
		} },
		.trajectoryType = TrajectoryType::LinearForward, // 軌跡タイプ：直線前方
		.trajectoryParams = Game::Combat::Skill::Data::SkillTrajectory::LinearTrajectoryParams
		{
			.speed = 20.0f // 速度
		},
	};

	// スキルの防御仕様を定義
	projectile.defense = SkillDefenseDef{
		.superArmorType = Game::Combat::Skill::Data::SkillSuperArmorType::None, // スーパーアーマーの種類
		.guardType = Game::Combat::Skill::Data::SkillGuardType::None // ガードの有無
	};

	// スキルのコストを定義
	projectile.cost = SkillCostDef{
		.manaCost = 0.0f, // マナコスト
		.staminaCost = 0.0f, // スタミナコスト
		.healthCost = 0.0f // ヘルスコスト
	};

	// スキルのダメージ情報を定義
	projectile.damage = SkillDamageDef{
		.onHitEffect = Game::Combat::Skill::Data::OnHitEffectType::None, // ヒット時の効果
		.damageType = SkillDamageType::Physical, // ダメージタイプ
		.baseDamage = 15.0f, // 基本ダメージ
		.isPercentBased = false, // パーセントベースのダメージかどうか
	};

	// バフ・デバフ：なし

	db.AddSkill(projectile);


	SkillDefinition testMagic;
	testMagic.id = 3;
	testMagic.name = "Test Magic Skill";
	testMagic.category = SkillCategory::AreaOfEffect; // スキルのカテゴリを設定

	// スキルの実行モデルを定義
	testMagic.execution = SkillExecutionModelDef{
		.timing = SkillPhaseTiming{
			.castTime = 1.5f, // キャストタイム(発生までの時間)
			.recoveryTime = 0.5f, // リカバリータイム(スキル終了後の待機時間)
			.duration = 3.0f // actionの持続時間
		},
		.castSyncPolicy = SkillCastSyncPolicy::Synchronous, // キャスト状態と攻撃判定の同期
		.cancelMask = SkillCancelPhase::All, // 全てのフェーズでキャンセル可能
		.cancelableSkillIds = {}, // このスキル中にキャンセルして発動可能なスキルID
		.movementLock = SkillMovementLockPolicy::Free, // スキルの移動ロックタイプ：自由
		.lockFacingDirection = false, // 詠唱中の向き固定なし
		.charge = SkillChargeSpecDef{
			.isChargeSkill = false, // チャージ式スキルかどうか
			.chargeTime = 0.0f // チャージ時間（チャージ式スキルの場合）
		},
	};

	// スキルの攻撃仕様を定義
	testMagic.attackSpec = SkillAttackSpecDef{
		.triggerTiming = Game::Combat::Skill::Component::SkillExecutionEvent::OnEnterActive, // スキルのトリガータイミング
		.lifetime = SkillLifetimeSpecDef{
			.hitBoxlifetimePolicy = AttackLifeTimeMode::IndependentEntityLifetime, // フェーズと同期
			.duration = 5.0f, // スキルの持続時間
			.despawnFlags = SkillDespawnCondition::TimeElapsed // 時間経過で消滅
		},
		.shape = Game::Combat::Skill::Component::Attack2DShape{ Game::Combat::Skill::Component::Circle2DAttack
		{
			.center = CanonicalDefaults::kLocalCenterXZ, // 中心位置
			.radius = 15.0f // 半径
		} },
		.trajectoryType = TrajectoryType::None, // 軌跡タイプはなし
		.trajectoryParams = Game::Combat::Skill::Data::SkillTrajectory::StaticTrajectory{}, // 静的軌跡パラメータ
	};

	// スキルの防御仕様を定義
	testMagic.defense = SkillDefenseDef{
		.superArmorType = Game::Combat::Skill::Data::SkillSuperArmorType::None, // スーパーアーマーの種類
		.guardType = Game::Combat::Skill::Data::SkillGuardType::None // ガードの有無
	};

	// スキルのコストを定義
	testMagic.cost = SkillCostDef{
		.manaCost = 10.0f, // マナコスト
		.staminaCost = 0.0f, // スタミナコスト
		.healthCost = 0.0f // ヘルスコスト
	};

	// スキルのダメージ情報を定義
	testMagic.damage = SkillDamageDef{
		.onHitEffect = Game::Combat::Skill::Data::OnHitEffectType::Stun, // ヒット時の効果
		.damageType = SkillDamageType::Magical, // ダメージタイプ
		.baseDamage = 25.0f, // 基本ダメージ
		.isPercentBased = false // パーセントベースのダメージかどうか
	};

	testMagic.cooldown = 5.0f; // スキルの再使用待機時間
	db.AddSkill(testMagic);

	SkillDefinition testBuffSkill;
	testBuffSkill.id = 4;
	testBuffSkill.name = "Test Buff Skill";

	testBuffSkill.category = SkillCategory::Buff; // スキルのカテゴリを設定

	// スキルの実行モデルを定義
	testBuffSkill.execution = SkillExecutionModelDef{
		.timing = SkillPhaseTiming{
			.castTime = 0.5f, // キャストタイム(発生までの時間)
			.recoveryTime = 0.5f, // リカバリータイム(スキル終了後の待機時間)
			.duration = 2.0f // actionの持続時間
		},
		.castSyncPolicy = SkillCastSyncPolicy::Asynchronous, // キャスト状態と攻撃判定の同期
		.cancelMask = SkillCancelPhase::All, // 全てのフェーズでキャンセル可能
		.cancelableSkillIds = {}, // このスキル中にキャンセルして発動可能なスキルID
		.movementLock = SkillMovementLockPolicy::Free, // スキルの移動ロックタイプ：自由
		.lockFacingDirection = false, // 詠唱中の向き固定なし
		.charge = SkillChargeSpecDef{
			.isChargeSkill = false, // チャージ式スキルかどうか
			.chargeTime = 0.0f // チャージ時間（チャージ式スキルの場合）
		},
	};

	// スキルの攻撃仕様を定義
	testBuffSkill.attackSpec = SkillAttackSpecDef{
		.triggerTiming = Game::Combat::Skill::Component::SkillExecutionEvent::OnEnterActive, // スキルのトリガータイミング
		.lifetime = SkillLifetimeSpecDef{
			.hitBoxlifetimePolicy = AttackLifeTimeMode::IndependentEntityLifetime, // フェーズと同期
			.duration = 10.0f, // スキルの持続時間
			.despawnFlags = SkillDespawnCondition::TimeElapsed // 時間経過で消滅
		},
		.shape = Game::Combat::Skill::Component::Attack2DShape{ Game::Combat::Skill::Component::Circle2DAttack
		{
			.center = CanonicalDefaults::kLocalCenterXZ, // 中心位置
			.radius = 10.0f // 半径
		} },
		.trajectoryType = TrajectoryType::None, // 軌跡タイプはなし
		.trajectoryParams = Game::Combat::Skill::Data::SkillTrajectory::StaticTrajectory{}, // 静的軌跡パラメータ
	};

	// スキルの防御仕様を定義
	testBuffSkill.defense = SkillDefenseDef{
		.superArmorType = Game::Combat::Skill::Data::SkillSuperArmorType::None, // スーパーアーマーの種類
		.guardType = Game::Combat::Skill::Data::SkillGuardType::None // ガードの有無
	};

	// スキルのコストを定義
	testBuffSkill.cost = SkillCostDef{
		.manaCost = 5.0f, // マナコスト
		.staminaCost = 0.0f, // スタミナコスト
		.healthCost = 0.0f // ヘルスコスト
	};

	// スキルのダメージ情報を定義
	testBuffSkill.damage = SkillDamageDef{
		.onHitEffect = Game::Combat::Skill::Data::OnHitEffectType::None, // ヒット時の効果
		.damageType = SkillDamageType::None, // ダメージタイプ
		.baseDamage = 0.0f, // 基本ダメージ
		.isPercentBased = false // パーセントベースのダメージかどうか
	};

	// バフ効果定義 (別の場所へ移動したほうがいいかも)
	BuffModifierDef buffModifier;
	buffModifier.buffType = SkillStatusBuffType::MovementSpeedUp; // バフの種類
	buffModifier.percentBonus = 0.2f; // バフの効果量（20%の移動速度アップ）
	buffModifier.duration = 10.0f; // バフの持続時間（10秒）
	buffModifier.targetStat = StatusEffectTargetStat::MovementSpeed; // 対象のステータス

	// バフ・デバフの情報を定義
	testBuffSkill.buffEffect = BuffEffectDef{
		.buffName = "Movement Speed Buff", // バフの名前
		.modifiers = { buffModifier }, // バフの修飾子
		.applyTo = EffectApplicationTarget::AreaOfEffect // バフの適用対象（キャスター自身）
	};

	testBuffSkill.cooldown = 15.0f; // スキルの再使用待機時間
	// デバフ効果はなし
	db.AddSkill(testBuffSkill);


		//SkillDefinition slash;
		//slash.id = 1;
	//slash.name = "Basic Slash";

	//// スキルのコストを定義
	//slash.cost = SkillCost{
	//	.manaCost = 0.0f, // マナコスト
	//	.staminaCost = 0.0f, // スタミナコスト
	//	.healthCost = 0.0f // ヘルスコスト
	//};

	//// スキルのフェーズタイミングを定義
	//slash.phaseTiming = SkillPhaseTiming{
	//	.castTime = 0.3f, // キャストタイム(発生までの時間)
	//	.recoveryTime = 0.4f, // リカバリータイム(スキル終了後の待機時間)
	//	.duration = 1.0f, // actionの持続時間
	//};

	//// スキルのアーマー情報を定義
	//slash.armorInfo = SkillArmorInfo{
	//	.superArmorType = Game::Combat::Skill::Data::SkillSuperArmorType::None, // スーパーアーマーの種類
	//	.guardType = Game::Combat::Skill::Data::SkillGuardType::None // ガードの有無
	//};

	//// スキルのヒット効果を定義
	//slash.hitEffect = SkillHitEffect{
	//	.onHitEffect = Game::Combat::Skill::Data::OnHitEffectType::None, // ヒット時の効果
	//	.statusEffectType = Game::Combat::Skill::Data::SkillStatusEffectType::None // 付与する状態異常の種類
	//};

	//// スキルのチャージ情報を定義
	//slash.chargeInfo = SkillChargeInfo{
	//	.isChargeSkill = false, // チャージ式スキルかどうか
	//	.chargeTime = 0.0f // チャージ時間（チャージ式スキルの場合）
	//};

	//// スキルのキャンセル可能なフェーズを定義
	//slash.cancelBehavior = SkillCancelPhase::All; // 全てのフェーズでキャンセル可能
	//slash.cancelableSkillIds = { 2, 3 }; // このスキル中にキャンセルして発動可能なスキルID

	//// 判定と軌跡の定義
	//slash.shape = Game::Combat::Skill::Component::Attack2DShape{ Game::Combat::Skill::Component::Sector2DAttack
	//{
	//	.center = CanonicalDefaults::kLocalCenterXZ, // 中心位置
	//	. direction = CanonicalDefaults::kLocalForwardXZ, 
	//	. angle = 0.5236f, // 約57度
	//	. radius = 10.0f
	//} };
	//slash.trajectoryType = TrajectoryType::None;// 軌跡タイプはなし
	//slash.trajectoryParams = Game::Combat::Skill::Data::SkillTrajectory::StaticTrajectory{}; // 静的軌跡パラメータ

	//// スキルのキャスト状態と攻撃判定の同期／非同期を定義
	//slash.castSyncType = SkillCastSyncType::Synchronous; // キャスト状態と攻撃判定の同期
	//slash.movementLockType = SkillMovementLockType::Locked; // スキルの移動ロックタイプ：ロック
	//
	//// スキルのあたり判定の情報を設定
	//slash.attackTiming = Game::Combat::Skill::Data::SkillAttackTiming
	//{
	//	.attackLifeTimeMode = Game::Combat::Skill::Data::AttackLifeTimeMode::SyncWithSkillPhase, // 攻撃判定ライフタイムの管理方法
	//	.triggerTiming = Game::Combat::Skill::Data::SkillTriggerTiming::OnCastingEnd, // スキルのトリガータイミング
	//	.attackDuration = 1.0f, // 攻撃判定の生存時間（秒） (Projectile寿命，Dotの継続時間など)
	//};
	//db.AddSkill(slash);



	//// 投射物の基礎スキル定義
	//SkillDefinition projectile;
	//projectile.id = 2;
	//projectile.name = "Projectile Skill";

	//// スキルのコストを定義
	//projectile.cost = SkillCost{
	//	.manaCost = 0.0f, // マナコスト
	//	.staminaCost = 0.0f, // スタミナコスト
	//	.healthCost = 0.0f // ヘルスコスト
	//};

	//// スキルのフェーズタイミングを定義
	//projectile.phaseTiming = SkillPhaseTiming{
	//	.castTime = 0.3f, // キャストタイム(発生までの時間)
	//	.recoveryTime = 0.3f, // リカバリータイム(スキル終了後の待機時間)
	//	.duration = 2.0f, // actionの持続時間
	//};

	//// スキルのアーマー情報を定義
	//projectile.armorInfo = SkillArmorInfo{
	//	.superArmorType = Game::Combat::Skill::Data::SkillSuperArmorType::None, // スーパーアーマーの種類
	//	.guardType = Game::Combat::Skill::Data::SkillGuardType::None // ガードの有無
	//};

	//// スキルのヒット効果を定義
	//projectile.hitEffect = SkillHitEffect{
	//	.onHitEffect = Game::Combat::Skill::Data::OnHitEffectType::None, // ヒット時の効果
	//	.statusEffectType = Game::Combat::Skill::Data::SkillStatusEffectType::None // 付与する状態異常の種類
	//};

	//// スキルのチャージ情報を定義
	//projectile.chargeInfo = SkillChargeInfo{
	//	.isChargeSkill = false, // チャージ式スキルかどうか
	//	.chargeTime = 0.0f // チャージ時間（チャージ式スキルの場合）
	//};

	//// スキルのキャンセル可能なフェーズを定義
	//projectile.cancelBehavior = SkillCancelPhase::All; // 全てのフェーズでキャンセル可能
	//projectile.cancelableSkillIds = {}; // このスキル中にキャンセルして発動可能なスキルID

	//// 判定と軌跡の定義
	//projectile.shape = Game::Combat::Skill::Component::Attack2DShape{ Game::Combat::Skill::Component::Circle2DAttack
	//{
	//	.center = CanonicalDefaults::kLocalCenterXZ, // 中心位置
	//	.radius = 5.0f // 半径
	//} };
	//projectile.trajectoryType = TrajectoryType::LinearForward; // 軌跡タイプ：直線前方
	//projectile.trajectoryParams = Game::Combat::Skill::Data::SkillTrajectory::LinearTrajectoryParams
	//{
	//	.speed = 20.0f // 速度
	//};

	//// スキルのキャスト状態と攻撃判定の同期／非同期を定義
	//projectile.castSyncType = SkillCastSyncType::Asynchronous; // キャスト状態と攻撃判定の非同期
	//projectile.movementLockType = SkillMovementLockType::Free; // スキルの移動ロックタイプ：自由

	//// スキルのあたり判定の情報を設定
	//projectile.attackTiming = Game::Combat::Skill::Data::SkillAttackTiming
	//{
	//	.attackLifeTimeMode = Game::Combat::Skill::Data::AttackLifeTimeMode::IndependentEntityLifetime, // 攻撃判定ライフタイムの管理方法
	//	.triggerTiming = Game::Combat::Skill::Data::SkillTriggerTiming::OnCastingEnd, // スキルのトリガータイミング
	//	.attackDuration = 5.0f, // 攻撃判定の生存時間（秒） (Projectile寿命，Dotの継続時間など)
	//};

	//db.AddSkill(projectile);


	//// テスト用スキル定義
	//SkillDefinition testSkill;
	//testSkill.id = 3;
	//testSkill.name = "Test Skill";

	//// スキルのコストを定義
	//testSkill.cost = SkillCost{
	//	.manaCost = 0.0f, // マナコスト
	//	.staminaCost = 0.0f, // スタミナコスト
	//	.healthCost = 0.0f // ヘルスコスト
	//};

	//// スキルのフェーズタイミングを定義
	//testSkill.phaseTiming = SkillPhaseTiming{
	//	.castTime = 0.5f, // キャストタイム(発生までの時間)
	//	.recoveryTime = 0.5f, // リカバリータイム(スキル終了後の待機時間)
	//	.duration = 1.0f, // actionの持続時間
	//};

	//// スキルのアーマー情報を定義
	//testSkill.armorInfo = SkillArmorInfo{
	//	.superArmorType = Game::Combat::Skill::Data::SkillSuperArmorType::Invincible, // スーパーアーマーの種類
	//	.guardType = Game::Combat::Skill::Data::SkillGuardType::None // ガードの有無
	//};

	//// スキルのヒット効果を定義
	//testSkill.hitEffect = SkillHitEffect{
	//	.onHitEffect = Game::Combat::Skill::Data::OnHitEffectType::Knockback, // ヒット時の効果
	//	.statusEffectType = Game::Combat::Skill::Data::SkillStatusEffectType::Burn// 付与する状態異常の種類
	//};

	//// スキルのチャージ情報を定義
	//testSkill.chargeInfo = SkillChargeInfo{
	//	.isChargeSkill = true, // チャージ式スキルかどうか
	//	.chargeTime = 1.0f // チャージ時間（チャージ式スキルの場合）
	//};

	//// スキルのキャンセル可能なフェーズを定義
	//testSkill.cancelBehavior = SkillCancelPhase::DuringCast | SkillCancelPhase::DuringRecovery; // キャスト中とリカバリー中にキャンセル可能

	//// 判定と軌跡の定義
	//testSkill.shape = Game::Combat::Skill::Component::Attack2DShape{ Game::Combat::Skill::Component::Rectangle2DAttack
	//{
	//	.center = glm::vec2(0.0f, 5.0f), // 中心位置
	//	.direction = CanonicalDefaults::kLocalForwardXZ, // 前方方向
	//	.width = 1.0f, // 幅
	//	.height = 10.0f // 高さ
	//} };
	//testSkill.trajectoryType = TrajectoryType::LinearForward; // 軌跡タイプ：自己中心の回転
	//testSkill.trajectoryParams = Game::Combat::Skill::Data::SkillTrajectory::RotateTrajectoryParams
	//{
	//	.startAngle = 60.0f, // 開始角度
	//	.endAngle = -60.0f // 終了角度
	//};

	//// スキルのキャスト状態と攻撃判定の同期／非同期を定義
	//testSkill.castSyncType = SkillCastSyncType::Synchronous; // キャスト状態と攻撃判定の同期
	//testSkill.movementLockType = SkillMovementLockType::Locked; // スキルの移動ロックタイプ：ロック

	//// スキルのあたり判定の情報を設定
	//testSkill.attackTiming = Game::Combat::Skill::Data::SkillAttackTiming
	//{
	//	.attackLifeTimeMode = Game::Combat::Skill::Data::AttackLifeTimeMode::SyncWithSkillPhase, // 攻撃判定ライフタイムの管理方法
	//	.triggerTiming = Game::Combat::Skill::Data::SkillTriggerTiming::OnCastingEnd, // スキルのトリガータイミング
	//	.attackDuration = 1.0f, // 攻撃判定の生存時間（秒） (Projectile寿命，Dotの継続時間など)
	//};

	//db.AddSkill(testSkill);

	//// テスト用スキル定義2
	//SkillDefinition testSkill2;
	//testSkill2.id = 4;
	//testSkill2.name = "Test Skill 2";

	//// スキルのコストを定義
	//testSkill2.cost = SkillCost{
	//	.manaCost = 0.0f, // マナコスト
	//	.staminaCost = 0.0f, // スタミナコスト
	//	.healthCost = 0.0f // ヘルスコスト
	//};

	//// スキルのフェーズタイミングを定義
	//testSkill2.phaseTiming = SkillPhaseTiming{
	//	.castTime = 1.0f, // キャストタイム(発生までの時間)
	//	.recoveryTime = 1.0f, // リカバリータイム(スキル終了後の待機時間)
	//	.duration = 2.0f, // actionの持続時間
	//};

	//// スキルのアーマー情報を定義
	//testSkill2.armorInfo = SkillArmorInfo{
	//	.superArmorType = Game::Combat::Skill::Data::SkillSuperArmorType::Unbreakable, // スーパーアーマーの種類
	//	.guardType = Game::Combat::Skill::Data::SkillGuardType::ForwardGuard// 前方ガード
	//};

	//// スキルのヒット効果を定義
	//testSkill2.hitEffect = SkillHitEffect{
	//	.onHitEffect = Game::Combat::Skill::Data::OnHitEffectType::Stun, // ヒット時の効果
	//	.statusEffectType = Game::Combat::Skill::Data::SkillStatusEffectType::Poison// 付与する状態異常の種類
	//};

	//// スキルのチャージ情報を定義
	//testSkill2.chargeInfo = SkillChargeInfo{
	//	.isChargeSkill = false, // チャージ式スキルかどうか
	//	.chargeTime = 0.0f // チャージ時間（チャージ式スキルの場合）
	//};

	//// スキルのキャンセル可能なフェーズを定義
	//testSkill2.cancelBehavior = SkillCancelPhase::DuringActive; // アクティブ中にキャンセル可能

	//// 判定と軌跡の定義
	//testSkill2.shape = Game::Combat::Skill::Component::Attack2DShape{ Game::Combat::Skill::Component::Circle2DAttack
	//{
	//	.center = CanonicalDefaults::kLocalCenterXZ, // 中心位置
	//	.radius = 15.0f // 半径
	//} };
	//testSkill2.trajectoryType = TrajectoryType::None; // 軌跡タイプ：自己中心の回転
	//testSkill2.trajectoryParams = Game::Combat::Skill::Data::SkillTrajectory::StaticTrajectory{}; // 静的軌跡パラメータ

	//// スキルのキャスト状態と攻撃判定の同期／非同期を定義
	//testSkill2.castSyncType = SkillCastSyncType::Synchronous; // キャスト状態と攻撃判定の同期
	//testSkill2.movementLockType = SkillMovementLockType::Locked; // スキルの移動ロックタイプ：Locked

	//// スキルのあたり判定の情報を設定
	//testSkill2.attackTiming = Game::Combat::Skill::Data::SkillAttackTiming
	//{
	//	.attackLifeTimeMode = Game::Combat::Skill::Data::AttackLifeTimeMode::IndependentEntityLifetime, // 攻撃判定ライフタイムの管理方法
	//	.triggerTiming = Game::Combat::Skill::Data::SkillTriggerTiming::OnCastingEnd, // スキルのトリガータイミング
	//	.attackDuration = 5.0f, // 攻撃判定の生存時間（秒） (Projectile寿命，Dotの継続時間など)
	//};

	//db.AddSkill(testSkill2);

	//Game::Combat::Skill::Data::SkillDefinition slash;
	//slash.id = 1;
	//slash.name = "Basic Slash";
	//slash.shape = Game::Combat::Skill::Component::Attack2DShape{ Game::Combat::Skill::Component::Circle2DAttack{CanonicalDefaults::kLocalCenterXZ, 5.0f} };
	//slash.duration = 1.0f;
	//slash.trajectoryType = Game::Combat::Skill::Data::TrajectoryType::LinearForward;
	//slash.trajectoryParams = Game::Combat::Skill::Data::SkillTrajectory::LinearTrajectoryParams
	//{
	//	.speed = 20.0f
	//};
	//slash.cancelBehavior = Game::Combat::Skill::Data::SkillCancelPhase::All;// 全てのフェーズでキャンセル可能
	//slash.castSyncType = Game::Combat::Skill::Data::SkillCastSyncType::Asynchronous;// キャスト状態と攻撃判定の非同期
	//slash.superArmorType = Game::Combat::Skill::Data::SkillSuperArmorType::None;// スーパーアーマーなし
	//slash.guardType = Game::Combat::Skill::Data::SkillGuardType::None;// ガードなし
	//slash.onHitEffect = Game::Combat::Skill::Data::OnHitEffectType::None;// ヒット時の効果なし
	//slash.statusEffectType = Game::Combat::Skill::Data::SkillStatusEffectType::Burn;// 付与する状態異常: 火傷
	//slash.cancelableSkillIds = { 2, 3 };// このスキル中にキャンセルして発動可能なスキルID
	//db.AddSkill(slash);

	//Game::Combat::Skill::Data::SkillDefinition slash2;
	//slash2.id = 2;
	//slash2.name = "Power Slash";
	//slash2.shape = Game::Combat::Skill::Component::Attack2DShape{ Game::Combat::Skill::Component::Sector2DAttack{
	//	.center = CanonicalDefaults::kLocalCenterXZ,
	//	. direction = CanonicalDefaults::kLocalForwardXZ, 
	//	. angle = 0.5236f, // 約57度
	//	. radius = 10.0f
	//} };// -Z方向が前方
	//slash2.duration = 1.0f;
	//slash2.cancelBehavior = Game::Combat::Skill::Data::SkillCancelPhase::DuringRecovery;// リカバリー中にキャンセル可能
	//slash2.castSyncType = Game::Combat::Skill::Data::SkillCastSyncType::Synchronous;// キャスト状態と攻撃判定の同期
	//slash2.superArmorType = Game::Combat::Skill::Data::SkillSuperArmorType::Invincible;// スーパーアーマー：無敵状態
	//slash2.guardType = Game::Combat::Skill::Data::SkillGuardType::ForwardGuard;// 前方ガード
	//slash2.onHitEffect = Game::Combat::Skill::Data::OnHitEffectType::Stun;// ヒット時の効果：スタン
	//slash2.statusEffectType = Game::Combat::Skill::Data::SkillStatusEffectType::None;// 付与する状態異常: なし
	//slash2.cancelableSkillIds = { 1, 3 };// このスキル中にキャンセルして発動可能なスキルID
	//db.AddSkill(slash2);

	//Game::Combat::Skill::Data::SkillDefinition blade;
	//blade.id = 3;
	//blade.name = "Blade";
	//blade.shape = Game::Combat::Skill::Component::Attack2DShape{ Game::Combat::Skill::Component::Rectangle2DAttack{glm::vec2(0.0f, 5.0f), CanonicalDefaults::kLocalForwardXZ, 1.0f, 10.0f} };
	//blade.duration = 1.0f;
	//// スキル奇跡の抽象定義の選択
	//blade.trajectoryType = Game::Combat::Skill::Data::TrajectoryType::RotateAroundSelf;
	//blade.trajectoryParams = Game::Combat::Skill::Data::SkillTrajectory::RotateTrajectoryParams
	//{// 関数定義
	//	.startAngle = 60.0f,
	//	.endAngle = -60.0f
	//};
	//blade.cancelBehavior = Game::Combat::Skill::Data::SkillCancelPhase::DuringCast | Game::Combat::Skill::Data::SkillCancelPhase::DuringActive;// キャスト中とアクティブ中にキャンセル可能
	//blade.castSyncType = Game::Combat::Skill::Data::SkillCastSyncType::Synchronous;// キャスト状態と攻撃判定の同期
	//blade.superArmorType = Game::Combat::Skill::Data::SkillSuperArmorType::None;// スーパーアーマーなし
	//blade.guardType = Game::Combat::Skill::Data::SkillGuardType::None;// なし
	//blade.onHitEffect = Game::Combat::Skill::Data::OnHitEffectType::Knockback;// ヒット時の効果：ノックバック
	//blade.statusEffectType = Game::Combat::Skill::Data::SkillStatusEffectType::Poison;// 付与する状態異常: 毒
	//blade.cancelableSkillIds = { 1, 2 };// このスキル中にキャンセルして発動可能なスキルID
	//db.AddSkill(blade);


	std::cout << "[InitializeSkills.cpp(skill register)]: Skill definition and registration in database completed successfully\n";
}