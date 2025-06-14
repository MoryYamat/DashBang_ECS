#include "InitCollisionResultBuffer.h"

#include "Game/Collision/Data/CollisionResultStorage.h"

#include "Common/GameNamespaceDecl.h"

#include <cassert>

void Game::Collision::Init::InitCollisionResultBuffer(eNsECS::EntityMgr& ecs)
{
	auto& buffer = ecs.createResource<gNsCollData::CollisionResultBuffer>();
}