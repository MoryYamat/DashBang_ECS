#pragma once 


#include "Common/EngineNamespaceDecl.h"
#include "Engine/ECS/EntityManager.h"

namespace Game::Collision::System
{
	// ECSグローバルリソースにバッファ情報を作成
	void InitCollisionResultStorage(eNsECS::EntityMgr ecs);

	// バッファをクリア
	void ClearCollisionResultStorage(eNsECS::EntityMgr& ecs);

	// バッファを更新
	void UpdateCollisionResultStorage(eNsECS::EntityMgr& ecs); 
}