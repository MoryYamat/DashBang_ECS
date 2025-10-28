#pragma once

#include <span>
#include <cstdint>

#include "Engine/FSM/Public/Core/Types.hpp"

namespace Engine::FSM::Core
{
	struct TransitionView_CSR
	{
		std::span<const Transition> edges;
		std::span<const std::uint32_t> headIndex;// [stateCount + 1]

		std::span<const Transition> outgoing(StateID s) const noexcept
		{
			const auto b = headIndex[s.id];
			const auto e = headIndex[s.id + 1];
			return { edges.data() + b, static_cast<size_t>(e - b) };
		}
	};

	inline TransitionView_CSR MakeTransitionView(const CanonicalAxis& ca)noexcept
	{
		return TransitionView_CSR{
			std::span<const Transition>(ca.edges.data(), ca.edges.size()),
			std::span<const std::uint32_t>(ca.headIndex.data(), ca.headIndex.size())
		};
	}
}