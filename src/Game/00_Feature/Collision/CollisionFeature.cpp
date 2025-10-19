#include "CollisionFeature.h"

#include "Game/Collision/System/CollisionDetectionSystem.h"

#include "Game/Collision/System/CollisionSyncSystem.h"

#include "Game/Collision/Init/InitCollisionResultBuffer.h"



// Collision バッファの初期化
void Game::Feature::CollisionFeature::InitCollisionBuffer(Engine::ECS::EntityMgr& ecs)
{
	Game::Collision::Init::InitCollisionResultBuffer(ecs);
}

// Collision バッファの更新
void Game::Feature::CollisionFeature::UpdateCollisionBuffer(Engine::ECS::EntityMgr& ecs)
{
	Game::Collision::System::UpdateCollisionResultBuffer(ecs);
}

// おそらく未使用：削除予定
// 論理レイヤー内でオブジェクト位置情報などと，コリジョン形状の位置情報などを同期させる処理
void Game::Feature::CollisionFeature::SyncLogicCollision(Engine::ECS::EntityMgr& ecs)
{
	// 論理レイヤー内でオブジェクト位置情報などと，コリジョン形状の位置情報などを同期
	Game::Collision::System::CollisionSyncSystem(ecs);
}