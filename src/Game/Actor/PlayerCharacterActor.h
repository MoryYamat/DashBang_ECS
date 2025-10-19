// test player character actor
// Class that creates an entity

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/Graphics/Renderer/Shader.h"


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

