//

#pragma once



#include "Engine/ECS/EntityManager.h"

namespace Game::Feature
{
	class CollisionFeature
	{
	public:
		// Collision バッファの初期化
		static void InitCollisionBuffer(Engine::ECS::EntityMgr& ecs);

		// Collision バッファの更新
		static void UpdateCollisionBuffer(Engine::ECS::EntityMgr& ecs);


		// CollisionCompのデータ更新
		static void SyncLogicCollision(Engine::ECS::EntityMgr& ecs);
};
}