#include "LogicLayerFeature.h"

#include "Game/00_Feature/Private/Character/Movement/MovementFeature.h"

#include "Game/00_Feature/Private/Collision/CollisionFeature.h"

#include "Game/00_Feature/Private/Combat/HitEvent/HitEventFeature.hpp"

#include "Game/00_Feature/Private/Combat/Skill/SkillFeature.h"

#include "Game/00_Feature/Private/Character/State/CharacterStateFeature.hpp"

#include "Game/00_Feature/Private/Character/Control/Skill/CharacterSkillFeature.hpp"

#include "Game/00_Feature/Private/ECS/ECSGeneral.hpp"

#include "Game/00_Feature/Private/System/WorldClockFeature.hpp"

// world
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Sync/Public/SyncApi.hpp"

// FIXME: LogicはWorldClockのScaledTimeを使うように変更する
// FIXME: 物理は固定ステップ(fixedStep = 1/60), FSM/AI/アニメ補完/AntiChain/UI切替/演出(ScaledTime)
void Game::Layer::LogicLayerFeature::Update(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Game::Feature;
	// world clock
	// System::WorldClockFeature::Update(ecs, deltaTime);

	// ------------------------- キャラクター関連処理 -------------------------
	// キャラクターの向きと位置を更新する
	// Game::Feature::Character::MovementFeature::UpdateLogicFacing(ecs);
	// Game::Feature::Character::MovementFeature::UpdateLogicPosition(ecs, deltaTime);


	// ------------------------- スキル関連処理 -------------------------
	// Intentに応じてスキルインスタンスを生成
	// Game::Feature::Combat::SkillFeature::TriggerSkillsFromIntent(ecs);
	// Game::Feature::Character::CharacterSkillFeature::UpdateCharacterSkillExecution(ecs, deltaTime);

	// スキルフェーズ(寿命管理と判定生成)
	// Game::Feature::Combat::SkillFeature::UpdateSkillPhaseSystem(ecs, deltaTime);
	// 検討必要：ここでキャラクターのスキル実行状態を更新するかどうか
	// 検討必要：ここでキャラクターのスキル実行状態を更新するかどうか
	// 検討必要：ここでキャラクターのスキル実行状態を更新するかどうか
	// stateへ移設
	// Game::Feature::Character::StateFeature::UPdateCharacterSkillExecutionState(ecs, deltaTime);

	// ===========================上：削除予定：FSM導入後廃止================
	
	// スキル軌跡更新
	// Game::Feature::Combat::SkillFeature::UpdateSkillTrajectorySystem(ecs, deltaTime);
	// hitbox lifetime 
	// Game::Feature::Combat::SkillFeature::UpdateHitoboxLifetimeSystem(ecs, deltaTime);

	// 削除予定：SkillExecutionをcharacterアクターへ付与する方式に変更したため
	// 最新：SkillExecutionLifetime
	//Game::Feature::Combat::SkillFeature::UpdateSkillExecutionLifetimeSystem(ecs);


	// lifetime
	// Game::Feature::ECS::GameGeneralSytem::UpdateLifetimeSystem(ecs, deltaTime); // ECSのライフタイムシステムを更新

	// ------------------------- コリジョン関連処理 -------------------------
	// コリジョンの位置情報などを更新する(CollisionComp.center etc.) (コリジョンはローカル形状情報に責務分離したためSyncCollは不要)
	// コリジョンの位置情報などを更新する(CollisionComp.center etc.) (コリジョンはローカル形状情報に責務分離したためSyncCollは不要)
	// コリジョンの位置情報などを更新する(CollisionComp.center etc.) (コリジョンはローカル形状情報に責務分離したためSyncCollは不要)
	// Game::Feature::CollisionFeature::SyncLogicCollision(ecs);
	// ===========================上：削除予定：FSM導入後廃止================

	// Game::Feature::CollisionFeature::UpdateCollisionBuffer(ecs);

	// Game::Feature::Combat::HitEventFeature::UpdateHitEventManager(ecs);
}

void Game::Layer::LogicLayerFeature::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	Game::Feature::Character::MovementFeature::UpdateLogicPosition(ctx);
	Engine::Sync::Apply2DTransform(ctx);
	Game::Feature::CollisionFeature::UpdateCollisionSystem(ctx);
}