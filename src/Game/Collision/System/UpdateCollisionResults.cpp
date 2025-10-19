// 現在未使用 削除予定

#include "UpdateCollisionResults.h"



#include "Game/Collision/Data/CollisionResultStorage.h"

// ECSグローバルリソースにバッファ情報を作成
void Game::Collision::System::InitCollisionResultStorage(Engine::ECS::EntityMgr ecs)
{
	auto& buffer = ecs.createResource <Game::Collision::Data::CollisionResultStorage>();
}

// バッファをクリア
void Game::Collision::System::ClearCollisionResultStorage(Engine::ECS::EntityMgr& ecs)
{
	auto& buffer = ecs.getResource<Game::Collision::Data::CollisionResultStorage>();

	buffer.Clear();
}

// バッファを更新
void Game::Collision::System::UpdateCollisionResultStorage(Engine::ECS::EntityMgr& ecs)
{
	auto& buffer = ecs.getResource<Game::Collision::Data::CollisionResultStorage>();

	// 計算関数を書く


	// buffer.add
}