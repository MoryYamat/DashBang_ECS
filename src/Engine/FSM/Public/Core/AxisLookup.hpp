#pragma once

#include "Types.hpp"
#include <cassert>
#include <string_view>


namespace Engine::FSM::Core::AxisLookup
{

	static constexpr std::uint32_t kInvalid = UINT32_MAX;

	inline std::uint32_t FindStateIdx(const CanonicalAxis& ax, std::string_view name) noexcept
	{
		if (auto it = ax.stateIdxByName.find(std::string(name)); it != ax.stateIdxByName.end())
			return it->second;

		assert(false && "AxisLookup::State: unknown state name");
		return kInvalid;
	}

    inline std::uint32_t FindCondIdx(const CanonicalAxis& ax, std::string_view name) noexcept {
        if (auto it = ax.condIdxByName.find(std::string(name)); it != ax.condIdxByName.end())
            return it->second;
        assert(false && "AxisLookup::Cond: unknown cond name");
        return kInvalid;
    }

    inline std::uint32_t FindSlotIdx(const CanonicalAxis& ax, std::string_view name) noexcept {
        if (auto it = ax.slotIdxByName.find(std::string(name)); it != ax.slotIdxByName.end())
            return it->second;
        assert(false && "AxisLookup::Slot: unknown slot name");
        return kInvalid;
    }

    inline std::uint32_t FindProfileIdx(const CanonicalAxis& ax, std::string_view name) noexcept {
        if (auto it = ax.profileIdxByName.find(std::string(name)); it != ax.profileIdxByName.end())
            return it->second;
        assert(false && "AxisLookup::Profile: unknown profile name");
        return kInvalid;
    }

    inline std::uint32_t FindFSMIdx(const CanonicalAxis& ax, std::string_view name) noexcept {
        if (auto it = ax.fsmIdxByName.find(std::string(name)); it != ax.fsmIdxByName.end())
            return it->second;
        assert(false && "AxisLookup::FSM: unknown fsm name");
        return kInvalid;
    }
}