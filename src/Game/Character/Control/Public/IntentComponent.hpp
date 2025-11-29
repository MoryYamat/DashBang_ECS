#pragma once

#include "Engine/ECS/Public/Entity.hpp"
#include "Game/Combat/Skill/Public/SkillFwd.hpp"

#include <vector>
#include <glm/glm.hpp>

namespace Game::Character::Control
{
	// intent component
	struct SkillRequest
	{
		Game::Combat::Skill::SkillID skill;
		Engine::ECS::Core::Entity caster;
		glm::vec2 targetPos;
		Engine::ECS::Core::Entity targetEntity;
		bool isQueued;
	};

	struct SkillIntentComponent
	{
		std::vector<SkillRequest> requests;
	};

}