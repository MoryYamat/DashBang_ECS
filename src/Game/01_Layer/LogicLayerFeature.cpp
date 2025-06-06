#inclu7de "LogicLayerFeature.h"

#include "Game/00_Feature/Movement/MovementFeature.h"

#include "Game/00_Feature/Collision/CollisionFeature.h"

#include "Game/00_Feature/Skill/SkillFeature.h"


void Game::Layer::LogicLayerFeature::Update(eNsECS::EntityMgr& ecs, float deltaTime, gNsSkillData::SkillDatabase& skillDB)
{
	// キャラクターの向きと位置を更新する
	gNsFeature::MovementFeature::UpdateLogicFacing(ecs);
	Game::Feature::MovementFeature::UpdateLogicPosition(ecs, deltaTime);

	// Skill
	gNsFeature::SkillFeature::SkillGenerateSystem(ecs, skillDB);
	78
	// コリジョンの位置情報などを更新する(CollisionComp.center etc.)
	gNsFeature::CollisionFeature::SyncLogicCollision(ecs);
}