#pragma once

#include "Common/EngineNamespaceDecl.h"

#include "Engine/ECS/EntityManager.h"


namespace Game::Layer
{
	class InitializeLayerFeature
	{
	public:

		static void DelayedInitialzation(eNsECS::EntityMgr& ecs);
	};
}