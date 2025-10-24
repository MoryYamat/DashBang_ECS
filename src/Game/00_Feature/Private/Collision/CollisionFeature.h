//

#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"


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



		//

		// Collision バッファの初期化
		static void InitCollisionBuffer(Engine::WorldSystem::Core::WorldCtx& ctx);

		// Collision バッファの更新
		static void UpdateCollisionBuffer(Engine::WorldSystem::Core::WorldCtx& ctx);


		// CollisionCompのデータ更新
		static void SyncLogicCollision(Engine::WorldSystem::Core::WorldCtx& ctx);
};
}