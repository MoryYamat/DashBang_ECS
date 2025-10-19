//

#pragma once

#include <glm/glm.hpp>

namespace Game::Character::Intent
{
	struct FacingIntentComponent
	{
		glm::vec2 front = glm::vec2(0.0f, 1.0f); // 正面の方向
		bool isValid = false; //入力が有効かどうか
	};
}