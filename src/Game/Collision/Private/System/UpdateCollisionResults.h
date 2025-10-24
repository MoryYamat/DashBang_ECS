#pragma once 



#include "Engine/ECS/EntityManager.h"

namespace Game::Collision::System
{
	// ECSグローバルリソースにバッファ情報を作成
	void InitCollisionResultStorage(Engine::ECS::EntityMgr ecs);

	// バッファをクリア
	void ClearCollisionResultStorage(Engine::ECS::EntityMgr& ecs);

	// バッファを更新
	void UpdateCollisionResultStorage(Engine::ECS::EntityMgr& ecs); 
}