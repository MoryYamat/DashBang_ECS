#include "CollisionFeature.h"



#include "Game/Collision/System/CollisionSyncSystem.h"

#include "Common/GameNamespaceDecl.h"


// Collision バッファの初期化
void Game::Feature::CollisionFeature::InitCollisionBuffer(eNsECS::EntityMgr& ecs)
{

}

// Collision バッファの更新
void Game::Feature::CollisionFeature::UpdateCollisionBuffer(eNsECS::EntityMgr& ecs)
{

}

// 論理レイヤー内でオブジェクト位置情報などと，コリジョン形状の位置情報などを同期させる処理
void Game::Feature::CollisionFeature::SyncLogicCollision(eNsECS::EntityMgr& ecs)
{
	// 論理レイヤー内でオブジェクト位置情報などと，コリジョン形状の位置情報などを同期
	gNsCollSystem::CollisionSyncSystem(ecs);
}