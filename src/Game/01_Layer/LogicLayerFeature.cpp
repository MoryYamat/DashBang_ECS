#include "LogicLayerFeature.h"

#include "Game/00_Feature/Character/Movement/MovementFeature.h"

#include "Game/00_Feature/Collision/CollisionFeature.h"

#include "Game/00_Feature/Combat/Skill/SkillFeature.h"

#include "Game/00_Feature/Character/State/CharacterStateFeature.hpp"

#include "Game/00_Feature/Character/Control/Skill/CharacterSkillFeature.hpp"

#include "Game/00_Feature/ECS/ECSGeneral.hpp"

#include "Game/00_Feature/System/WorldClockFeature.hpp"

// FIXME: LogicはWorldClockのScaledTimeを使うように変更する
// FIXME: 物理は固定ステップ(fixedStep = 1/60), FSM/AI/アニメ補完/AntiChain/UI切替/演出(ScaledTime)
void Game::Layer::LogicLayerFeature::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Game::Feature;
	// world clock
	System::WorldClockFeature::Update(ecs, deltaTime);

	// ------------------------- キャラクター関連処理 -------------------------
	// キャラクターの向きと位置を更新する
	gNsFeature::Character::MovementFeature::UpdateLogicFacing(ecs);
	Game::Feature::Character::MovementFeature::UpdateLogicPosition(ecs, deltaTime);


	// ------------------------- スキル関連処理 -------------------------
	// Intentに応じてスキルインスタンスを生成
	// gNsFeature::Combat::SkillFeature::TriggerSkillsFromIntent(ecs);
	// Game::Feature::Character::CharacterSkillFeature::UpdateCharacterSkillExecution(ecs, deltaTime);

	// スキルフェーズ(寿命管理と判定生成)
	// gNsFeature::Combat::SkillFeature::UpdateSkillPhaseSystem(ecs, deltaTime);
	// 検討必要：ここでキャラクターのスキル実行状態を更新するかどうか
	// 検討必要：ここでキャラクターのスキル実行状態を更新するかどうか
	// 検討必要：ここでキャラクターのスキル実行状態を更新するかどうか
	// stateへ移設
	// gNsFeature::Character::StateFeature::UPdateCharacterSkillExecutionState(ecs, deltaTime);

	// ===========================上：削除予定：FSM導入後廃止================
	
	// スキル軌跡更新
	gNsFeature::Combat::SkillFeature::UpdateSkillTrajectorySystem(ecs, deltaTime);
	// hitbox lifetime 
	gNsFeature::Combat::SkillFeature::UpdateHitoboxLifetimeSystem(ecs, deltaTime);

	// 削除予定：SkillExecutionをcharacterアクターへ付与する方式に変更したため
	// 最新：SkillExecutionLifetime
	//gNsFeature::Combat::SkillFeature::UpdateSkillExecutionLifetimeSystem(ecs);


	// lifetime
	gNsFeature::ECS::GameGeneralSytem::UpdateLifetimeSystem(ecs, deltaTime); // ECSのライフタイムシステムを更新

	// ------------------------- コリジョン関連処理 -------------------------
	// コリジョンの位置情報などを更新する(CollisionComp.center etc.) (コリジョンはローカル形状情報に責務分離したためSyncCollは不要)
	// コリジョンの位置情報などを更新する(CollisionComp.center etc.) (コリジョンはローカル形状情報に責務分離したためSyncCollは不要)
	// コリジョンの位置情報などを更新する(CollisionComp.center etc.) (コリジョンはローカル形状情報に責務分離したためSyncCollは不要)
	// gNsFeature::CollisionFeature::SyncLogicCollision(ecs);
	// ===========================上：削除予定：FSM導入後廃止================

	gNsFeature::CollisionFeature::UpdateCollisionBuffer(ecs);
}