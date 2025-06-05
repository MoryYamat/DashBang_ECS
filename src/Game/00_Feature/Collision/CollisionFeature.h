//

#pragma once

#include "Common/EngineNamespaceDecl.h"

#include "Engine/ECS/EntityManager.h"

namespace Game::Feature
{
	class CollisionFeature
	{
	public:
		static void SyncLogicCollision(eNsECS::EntityMgr& ecs);
	};
}