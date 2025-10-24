#pragma once

#include <optional>
#include <typeindex>
#include <cstdint>

namespace Game::Combat::Skill::FSM::StateModel
{
	struct SkillFSMTransitionRequest
	{
		std::optional<std::type_index> requestedTo = std::nullopt;
		uint32_t priority = 0;
	};

	struct SkillFSMTransitionRequestComponent
	{
		std::vector<SkillFSMTransitionRequest> requests;
	};
} 