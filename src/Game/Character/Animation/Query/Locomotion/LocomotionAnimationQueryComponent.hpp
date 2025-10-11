#pragma once

#include <GLM/glm.hpp>


namespace Game::Character::Animation::Query
{
	// locomotion
	struct LocomotionAnimQueryComponent
	{
		// movement
		bool isMoving = false;
		float moveSpeed = 0.0f;
		float normalizedSpeed = 0.0f;
		glm::vec2 moveDirWorld = { 0,1 };
		glm::vec2 facingDirWorld = { 0,1 };
		float moveYawLocal = 0.0f;// facing基準の角度 [-pi, pi]
		bool isLocomotionActive = false;

		// 
		bool valid = false;
	};
}