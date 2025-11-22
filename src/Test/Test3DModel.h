#pragma once

#include "Engine/ECS/EntityManager.h"

// #include "Engine/Graphics/Private/Renderer/Shader.h"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"




namespace Test::Actor
{
	class Test3DModel
	{
	public:
		Test3DModel(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Shader* shader);

		~Test3DModel();
	private:

	};
}

