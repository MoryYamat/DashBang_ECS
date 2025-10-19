// Setting context data for single search

#pragma once

#include "Engine/ECS/Entity.h"

#include <GLM/glm.hpp>

#include <optional>



namespace Game::Collision::Data
{
	struct PlayerCollisionContext
	{
		glm::vec2 center;
		float radius;
		std::optional<Engine::ECS::Entity> playerEntity;

		PlayerCollisionContext(

		)
		{ }
	};

	struct TileCollisionContext
	{
		glm::ivec2 tileIndex;
		std::optional<Engine::ECS::Entity> tileEntity;
	};
}