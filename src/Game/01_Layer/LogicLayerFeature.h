#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"


namespace Game::Layer
{
	class LogicLayerFeature
	{
	public:
		static void Update(eNsECS::EntityMgr& ecs, float deltaTime);
	};
}