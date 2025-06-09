//

#pragma once

#include "Common/EngineNamespaceDecl.h"

#include "Engine/ECS/EntityManager.h"

namespace Game::Feature
{
	class CollisionFeature
	{
	public:
		// Collision バッファの初期化
		static void InitCollisionBuffer(eNsECS::EntityMgr& ecs);

		// Collision バッファの更新
		static void UpdateCollisionBuffer(eNsECS::EntityMgr& ecs);


		// CollisionCompのデータ更新
		static void SyncLogicCollision(eNsECS::EntityMgr& ecs);
};
}