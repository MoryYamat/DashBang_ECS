// test player character actor
// Class that creates an entity

#pragma once

#include "Engine/ECS/EntityManager.h"

// #include "Engine/Graphics/Private/Renderer/Shader.h"

#include "Engine/Graphics/Public/GraphicsFwd.hpp"


namespace Game::Actor::Player
{
	class PlayerCharacter
	{
	public:
		PlayerCharacter(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Render::Shader* shader);

		~PlayerCharacter();

	private:


	};
}

