// Test Terrain Actor

#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/Graphics/Renderer/Shader.h"



namespace Game::Actor::Map
{
	class TestBaseTerrainActor
	{
	public:
		TestBaseTerrainActor(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Render::Shader* shader);
	};
}