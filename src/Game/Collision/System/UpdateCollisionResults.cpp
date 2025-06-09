#include "UpdateCollisionResults.h"

#include "Common/GameNamespaceDecl.h"

#include "Game/Collision/Data/CollisionResultStorage.h"

// ECSグローバルリソースにバッファ情報を作成
void Game::Collision::System::InitCollisionResultStorage(eNsECS::EntityMgr ecs)
{
	auto& buffer = ecs.createResource <gNsCollData::CollisionResultStorage>();
}

// バッファをクリア
void Game::Collision::System::ClearCollisionResultStorage(eNsECS::EntityMgr& ecs)
{
	auto& buffer = ecs.getResource<gNsCollData::CollisionResultStorage>();

	buffer.Clear();
}

// バッファを更新
void Game::Collision::System::UpdateCollisionResultStorage(eNsECS::EntityMgr& ecs)
{
	auto& buffer = ecs.getResource<gNsCollData::CollisionResultStorage>();

	// 計算関数を書く


	// buffer.add
}