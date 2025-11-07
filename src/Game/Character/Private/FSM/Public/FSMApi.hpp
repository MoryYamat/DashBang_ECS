#pragma once

#include "Engine/Public/EngineFwd.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"

#include "Engine/FSM/Public/Base/Types.hpp"
#include "Engine/FSM/Public/Base/FSMApi.hpp"

namespace Game::Character::FSM::Movement
{
	//void RegisterMovementAuthoring(Engine::FSM::Core::FSMRegistry& reg);

	// void BuildMovementCondTable_Stage(Engine::FSM::Core::CondTableStaging& stage);

	void SetupMovementAxisRuntime(Engine::WorldSystem::Core::WorldCtx& ctx);

	void makeAxis_Movement(Engine::FSM::Base::FSMRegistry& reg);
	void makeFSM_Movement(Engine::FSM::Base::FSMRegistry& reg);
	void RegisterMovementAxes(Engine::FSM::Base::FSMRegistry& reg);
	void ProvideMovementConds(const Engine::FSM::Base::FSMCatalog& cat,
		std::vector<Engine::FSM::Base::AxisCondBindings>& out);


}