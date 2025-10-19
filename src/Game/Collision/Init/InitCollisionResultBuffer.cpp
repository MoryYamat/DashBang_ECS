#include "InitCollisionResultBuffer.h"

#include "Game/Collision/Data/CollisionResultStorage.h"



#include <cassert>

void Game::Collision::Init::InitCollisionResultBuffer(Engine::ECS::EntityMgr& ecs)
{
	auto& buffer = ecs.createResource<Game::Collision::Data::CollisionResultBuffer>();
}