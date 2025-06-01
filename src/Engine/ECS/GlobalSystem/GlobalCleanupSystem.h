//

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Engine::ECS
{
	namespace GrobalSystem
	{
		void RunCleanup(eNsECS::EntityMgr& ecs);
	}
}
