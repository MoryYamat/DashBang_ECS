#pragma once

#include "Engine/FSM/Public/Core/AxisComponent.hpp"

#include "Game/Character/Private/FSM/Private/Movement/Public/MovementFieldReader.hpp"

namespace Game::Character::FSM::Movement
{
	struct MovementTag{};
	using MovementAxisComp = Engine::FSM::Core::AxisComponent<MovementTag, MovementFieldReader>;

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