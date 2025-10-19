#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/Graphics/Renderer/Shader.h"




namespace Test::Actor
{
	class Test3DModel
	{
	public:
		Test3DModel(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Render::Shader* shader);

		~Test3DModel();
	private:

	};
}

