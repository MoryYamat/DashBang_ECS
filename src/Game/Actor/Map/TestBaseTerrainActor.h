// Test Terrain Actor

#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/Graphics/Renderer/Shader.h"

#include "Common/EngineNamespaceDecl.h"


namespace Game::Actor::Map
{
	class TestBaseTerrainActor
	{
	public:
		TestBaseTerrainActor(eNsECS::EntityMgr& ecs, eNsGfxRender::Shader* shader);
	};
}