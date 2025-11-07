#include "Engine/FSM/Public/Base/DTO.hpp"
#include "Engine/FSM/Public/Base/Types.hpp"

#include "Engine/FSM/Public/Base/Registry.hpp"

namespace Game::Character::FSM::Movement
{
	using namespace Engine::FSM::Base;

	void makeAxis_Movement(FSMRegistry& reg)
	{
		AxisDTO ax;
		ax.axis = "Movement";
		ax.stateU = { "Idle", "Moving" };
		ax.condU = { "canMove", "shouldStop"};
		ax.slotU = { "Transition", "Stop"};
		ax.profileU = { "Default" };
		ax.version = 1;
		reg.add(std::move(ax));
	}

	void makeFSM_Movement(FSMRegistry& reg)
	{
		FSMDTO f;
		f.axis = "Movement";
		f.fsm = "Basic";
		f.states = { "Idle", "Moving" };
		f.profiles = { "Default" };

		// profile defs
		f.profile_defs.push_back(ProfileDefDTO{
			.name = "Default",
			.extends = "",
			.binds = {
				{ "Transition", "canMove"},
				{"Stop", "shouldStop"}
			} 
			});

		// transitions
		f.transitions.push_back({ "Idle", "Moving", "Transition", 0 });
		f.transitions.push_back({ "Moving", "Idle", "Stop", 0 });

		f.version = 1;

		reg.add(std::move(f));
	}

	void RegisterMovementAxes(Engine::FSM::Base::FSMRegistry& reg)
	{
		makeAxis_Movement(reg);
		makeFSM_Movement(reg);
	}
}