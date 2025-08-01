#pragma once

#include <GLM/glm.hpp>

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"
namespace Game::Character::FSM::Movement
{
	struct MovementFSMContext
	{
		float elapsedTime = 0.0f;
		float phaseElapsedTime = 0.0f;

		glm::vec2 direction = glm::vec2(0.0f);// MovementIntent‚©‚çæ“¾
		bool intentActive = false;


		glm::vec2 velocity;// íœ—\’èF”p~
	};
}