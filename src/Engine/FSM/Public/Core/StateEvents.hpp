#pragma once

#include "Engine/FSM/Public/FSMFwd.hpp"

#include <cstdint>

namespace Engine::FSM::Core
{
	struct StateEventView
	{
		StateID prev;
		StateID cur;
		bool changed;

		StateEventView(StateID prev, StateID cur, bool changed) :
			prev(prev), cur(cur), changed(changed) {};

		// チャタリング防止必要かも?
		bool whileIn(StateID s) const noexcept
		{
			return cur == s;
		}

		bool onEnter(StateID s) const noexcept
		{
			return changed && cur == s;
		}

		bool onExit(StateID s) const noexcept
		{
			return changed && prev == s;
		}
	};

	// トリガ種別
	enum class StateTriggerKind : std::uint8_t
	{
		WhileIn,
		OnEnter,
		OnExit,
	};

	//template<typename ID>
	//struct EventView
	//{
	//	ID prev;
	//	ID cur;
	//	bool changed;

	//	bool whileIn(ID v) const noexcept
	//	{
	//		return cur == v;
	//	};

	//	bool onEnter(ID v) const noexcept
	//	{
	//		return changed && cur == v;
	//	};

	//	bool onExit(ID v) const noexcept
	//	{
	//		return changed && prev == v;
	//	};
	//};

	//using StateEventView = EventView<StateID>;
	//using ProfileEventView = EventView<ProfileID>;
}