#pragma once

#include <GLM/glm.hpp>

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"
namespace Game::Character::FSM::Movement
{
	struct MovementContext
	{
		glm::vec2 velocity;
	};
}