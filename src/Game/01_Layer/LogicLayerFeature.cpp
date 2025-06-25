#include "LogicLayerFeature.h"

#include "Game/00_Feature/Character/Movement/MovementFeature.h"

#include "Game/00_Feature/Collision/CollisionFeature.h"

#include "Game/00_Feature/Combat/Skill/SkillFeature.h"

#include "Game/00_Feature/Character/State/CharacterStateFeature.hpp"

#include "Game/00_Feature/Character/Control/Skill/CharacterSkillFeature.hpp"

void Game::Layer::LogicLayerFeature::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// ------------------------- キャラクター関連処理 -------------------------
	// キャラクターの向きと位置を更新する
	gNsFeature::Character::MovementFeature::UpdateLogicFacing(ecs);
	Game::Feature::Character::MovementFeature::UpdateLogicPosition(ecs, deltaTime);

	// ------------------------- スキル関連処理 -------------------------
	// Intentに応じてスキルインスタンスを生成
	// gNsFeature::Combat::SkillFeature::TriggerSkillsFromIntent(ecs);
	Game::Feature::Character::CharacterSkillFeature::UpdateCharacterSkillExecution(ecs, deltaTime);

	// スキルフェーズ(寿命管理と判定生成)
	gNsFeature::Combat::SkillFeature::UpdateSkillPhaseSystem(ecs, deltaTime);

	// 検討必要：ここでキャラクターのスキル実行状態を更新するかどうか
	// 検討必要：ここでキャラクターのスキル実行状態を更新するかどうか
	// 検討必要：ここでキャラクターのスキル実行状態を更新するかどうか
	gNsFeature::Character::StateFeature::UPdateCharacterSkillExecutionState(ecs, deltaTime);


	// スキル軌跡更新
	gNsFeature::Combat::SkillFeature::UpdateSkillTrajectorySystem(ecs, deltaTime);


	// ------------------------- コリジョン関連処理 -------------------------
	// コリジョンの位置情報などを更新する(CollisionComp.center etc.) (コリジョンはローカル形状情報に責務分離したためSyncCollは不要)
	// コリジョンの位置情報などを更新する(CollisionComp.center etc.) (コリジョンはローカル形状情報に責務分離したためSyncCollは不要)
	// コリジョンの位置情報などを更新する(CollisionComp.center etc.) (コリジョンはローカル形状情報に責務分離したためSyncCollは不要)
	// gNsFeature::CollisionFeature::SyncLogicCollision(ecs);

	gNsFeature::CollisionFeature::UpdateCollisionBuffer(ecs);
}