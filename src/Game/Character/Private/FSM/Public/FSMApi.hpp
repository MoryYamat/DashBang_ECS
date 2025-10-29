#pragma once

#include "Engine/Public/EngineFwd.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"

namespace Game::Character::FSM::Movement
{
	void RegisterMovementAuthoring(Engine::FSM::Core::FSMRegistry& reg);

	void BuildMovementCondTable_Stage(Engine::FSM::Core::CondTableStaging& stage);
}