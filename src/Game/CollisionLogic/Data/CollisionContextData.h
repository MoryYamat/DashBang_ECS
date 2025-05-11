// Setting context data for single search

#pragma once

#include "Core/ECS/Entity.h"

#include <GLM/glm.hpp>

#include <optional>

namespace GameUtils::CollisionLogic::Data
{
	struct PlayerCollisionContext
	{
		glm::vec2 center;
		float radius;
		std::optional<Entity> playerEntity;

		PlayerCollisionContext(

		)
		{ }
	};

	struct TileCollisionContext
	{
		glm::ivec2 tileIndex;
		std::optional<Entity> tileEntity;
	};
}