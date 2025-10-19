#pragma once

#include "Game/Character/FSM/Interference/Core/Data/FSMInterferenceRequest.hpp"
#include <typeindex>
namespace Game::Character::FSM::Interference::Core::Util
{
	
	inline bool isTargetAxis(const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest& req
		, const std::type_index& targetAxis)
	{
		return req.targetAxis == targetAxis;
	}

	inline bool isValidRequest(const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest& req)
	{
		return req.durationSec > 0.0f;
	}

	//	
	inline bool shouldApply(const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest& req
		, const std::type_index& targetAxis)
	{
		return isValidRequest(req) && isTargetAxis(req, targetAxis);
	}

	inline bool isControlAllowed(const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest& req
		, Game::Character::FSM::Interference::Core::Data::ControlSeverity currentSeverity)
	{
		return req.severity > currentSeverity;
	}

	template<typename LeaseComponent>
	inline void updateLease(LeaseComponent& lease,
		const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest& req)
	{
		lease.severity = req.severity;
		lease.remainingDurationSec = req.durationSec;
	}

	template<typename StateComponent>
	inline void applyForcedState(StateComponent& state
		, const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest& req)
	{
		if (req.forcedState.has_value())
			state.current = *req.forcedState;
	}

	template<typename LeaseComponent, typename StateComponent>
	inline void applyForcedTransition(LeaseComponent& lease, StateComponent& state
		, const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest& req)
	{
		if (!req.forcedState.has_value()) return;

		updateLease(lease, req);
		applyForcedState(state, req);
	}
}