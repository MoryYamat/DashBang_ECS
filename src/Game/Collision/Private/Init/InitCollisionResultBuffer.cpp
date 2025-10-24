#include "InitCollisionResultBuffer.h"

#include "Game/Collision/Private/Data/CollisionResultStorage.h"



#include <cassert>

void Game::Collision::Init::InitCollisionResultBuffer(Engine::ECS::EntityMgr& ecs)
{
	auto& buffer = ecs.createResource<Game::Collision::Data::CollisionResultBuffer>();
}