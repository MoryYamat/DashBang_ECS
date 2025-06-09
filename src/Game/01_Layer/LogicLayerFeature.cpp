#include "LogicLayerFeature.h"

#include "Game/00_Feature/Character/Movement/MovementFeature.h"

#include "Game/00_Feature/Collision/CollisionFeature.h"

#include "Game/00_Feature/Combat/Skill/SkillFeature.h"


void Game::Layer::LogicLayerFeature::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// ------------------------- キャラクター関連処理 -------------------------
	// キャラクターの向きと位置を更新する
	gNsFeature::Character::MovementFeature::UpdateLogicFacing(ecs);
	Game::Feature::Character::MovementFeature::UpdateLogicPosition(ecs, deltaTime);

	// ------------------------- スキル関連処理 -------------------------
	// Intentに応じてスキルインスタンスを生成
	gNsFeature::Combat::SkillFeature::TriggerSkillsFromIntent(ecs);

	// スキルフェーズ(寿命管理と判定生成)
	gNsFeature::Combat::SkillFeature::UpdateSkillPhaseSystem(ecs, deltaTime);
	// スキル軌跡更新
	gNsFeature::Combat::SkillFeature::UpdateSkillTrajectorySystem(ecs, deltaTime);



	// ------------------------- コリジョン関連処理 -------------------------
	// コリジョンの位置情報などを更新する(CollisionComp.center etc.)
	gNsFeature::CollisionFeature::SyncLogicCollision(ecs);
}