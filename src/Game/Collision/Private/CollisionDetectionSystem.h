// The system responsible for "executing" hit detection
// 当たり判定の「実行」責任を持つシステム

#pragma once 

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

// 
namespace Game::Collision::System
{
	// void UpdateCollisionResultBuffer(Engine::ECS::EntityMgr& ecs);

	// ECSグローバルリソース導入後未使用(削除予定)
	// void UpdateCollisionResultStorage(Engine::ECS::EntityMgr& ecs, Game::Collision::Data::CollisionResultStorage& collisionResultStorage);


	 // void CollisionDetectionSystem(Engine::ECS::EntityMgr& ecs, Game::Collision::Data::CollisionResultStorage& collisionResultStorage);


	// player and tile map collision detection

	// bool isSkillEntity(Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity entity);
};

namespace Game::Collision
{
	void CollisionDetection(Engine::WorldSystem::Core::WorldCtx& ctx);
}