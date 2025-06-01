//

#pragma once

#include <GLM/glm.hpp>

namespace Game::Character::Intent
{
	struct MovementIntentComponent
	{
		glm::vec2 direction = glm::vec2(0.0f);	// ƒ[ƒ‹ƒh‹óŠÔ‚É‚¨‚¯‚é•ûŒü
		bool isActive = false;					// ˆÓ}‚Æ‚µ‚Ä‚ÌˆÚ“®‚Ì—L–³
	};
}