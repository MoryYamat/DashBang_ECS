#pragma once

#include "Game/Character/FSM/Interference/Core/Data/FSMInterferenceRequest.hpp"
#include <typeindex>
namespace Game::Character::FSM::Interference::Core::Util
{
	using namespace Game::Character::FSM::Interference::Core::Data;

	inline bool isTargetAxis(const FSMInterferenceRequest& req, const std::type_index& targetAxis)
	{
		return req.targetAxis == targetAxis;
	}

	inline bool isValidRequest(const FSMInterferenceRequest& req)
	{
		return req.durationSec > 0.0f;
	}

	//	
	inline bool shouldApply(const FSMInterferenceRequest& req, const std::type_index& targetAxis)
	{
		return isValidRequest(req) && isTargetAxis(req, targetAxis);
	}

	inline bool isControlAllowed(const FSMInterferenceRequest& req, ControlSeverity currentSeverity)
	{
		return req.severity > currentSeverity;
	}

	template<typename LeaseComponent>
	inline void updateLease(LeaseComponent& lease, const FSMInterferenceRequest& req)
	{
		lease.severity = req.severity;
		lease.remainingDurationSec = req.durationSec;
	}

	template<typename StateComponent>
	inline void applyForcedState(StateComponent& state, const FSMInterferenceRequest& req)
	{
		if (req.forcedState.has_value())
			state.current = *req.forcedState;
	}

	template<typename LeaseComponent, typename StateComponent>
	inline void applyForcedTransition(LeaseComponent& lease, StateComponent& state, const FSMInterferenceRequest& req)
	{
		if (!req.forcedState.has_value()) return;

		updateLease(lease, req);
		applyForcedState(state, req);
	}
}