#pragma once

#include <optional>
#include <typeindex>
#include <cstdint>

// TODO: •›ì—p‚Æ‚µ‚Ä‚ÌˆÚ“®
namespace Game::Character::FSM::Movement
{
	struct MovementFSMTransitionRequestComponent
	{
		std::optional<std::type_index> requestedTo = std::nullopt;
		uint32_t priority = 0;
	};
}