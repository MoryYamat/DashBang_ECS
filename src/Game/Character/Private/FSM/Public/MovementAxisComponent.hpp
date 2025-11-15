#pragma once

#include "Engine/FSM/Public/Core/AxisComponent.hpp"

#include "Game/Character/Private/FSM/Public/MovementFieldReader.hpp"

#include <cstdint>

namespace Game::Character::FSM::Movement
{
	struct MovementTag{};
	using MovementAxisComp = Engine::FSM::Core::AxisComponent<MovementTag, MovementFieldReader>;

	struct MovementStateComp
	{
		std::uint32_t curState = 0;
		std::uint32_t prevState = 0;

		bool changedThisFrame = false;
	};

}

namespace Engine::FSM::Core
{
	template<>
	struct AxisTraits<Game::Character::FSM::Movement::MovementTag>
	{
		static std::string_view AxisName() { return "Movement"; }
		static std::string_view DefaultFSMName() { return "Basic"; }

	};
}