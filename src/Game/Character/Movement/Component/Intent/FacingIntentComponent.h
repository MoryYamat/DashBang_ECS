//

#pragma once

namespace Game::Character::Intent
{
	struct FacingIntentComponent
	{
		glm::vec2 front = glm::vec2(0.0f, 1.0f); // ³–Ê‚Ì•ûŒü
		bool isValid = false; //“ü—Í‚ª—LŒø‚©‚Ç‚¤‚©
	};
}