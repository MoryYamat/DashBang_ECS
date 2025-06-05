#include "LogicLayerFeature.h"

#include "Game/00_Feature/MovementFeature.h"

#include "Game/00_Feature/Collision/CollisionFeature.h"

#include "Common/GameNamespaceDecl.h"

void Game::Layer::LogicLayerFeature::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// キャラクターの向きと位置を更新する
	gNsFeature::MovementFeature::UpdateLogicFacing(ecs);
	Game::Feature::MovementFeature::UpdateLogicPosition(ecs, deltaTime);

	// コリジョンの位置情報などを更新する(CollisionComp.center etc.)
	gNsFeature::CollisionFeature::SyncLogicCollision(ecs);
}