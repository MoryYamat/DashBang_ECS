#pragma once

#include <GLM/glm.hpp>

#include "Engine/ECS/EntityManager.h"


namespace Game::Character::FSM::Movement
{
	struct MovementFSMContext
	{
		float elapsedTime = 0.0f;
		float phaseElapsedTime = 0.0f;

		glm::vec2 direction = glm::vec2(0.0f);// MovementIntentから取得
		bool intentActive = false;


		glm::vec2 velocity;// 削除予定：廃止
	};
}