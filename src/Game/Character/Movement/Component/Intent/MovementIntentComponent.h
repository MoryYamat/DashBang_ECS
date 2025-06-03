//

#pragma once

#include <GLM/glm.hpp>

namespace Game::Character::Intent
{
	struct MovementIntentComponent
	{
		glm::vec2 direction = glm::vec2(0.0f);	// ワールド空間における方向// directionは使用側で正規化する
		bool isActive = false;					// 意図としての移動の有無
	};
}