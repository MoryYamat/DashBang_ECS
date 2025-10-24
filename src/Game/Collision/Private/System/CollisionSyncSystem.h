// inside Logic Layer 


#pragma once

#include "Engine/ECS/EntityManager.h"



// Collision情報をローカル形状情報にしたため，未使用(削除予定)
namespace Game::Collision::System
{
	void CollisionSyncSystem(Engine::ECS::EntityMgr& ecs);
}