#pragma once

#include <cstdint>
#include <vector>
#include <utility>

#include <span>

#include <unordered_map>

namespace Engine::FSM::Base
{
	template<typename T>
	struct StrongID 
	{ 
		std::uint32_t v = UINT32_MAX;
		constexpr bool valid() const 
		{
			return v != UINT32_MAX;
		}

		friend constexpr bool operator == (StrongID, StrongID) = default;
	};

	struct StateTag {}; using StateID = StrongID<StateTag>;
	struct CondTag {}; using CondID = StrongID<CondTag>;
	struct SlotTag {}; using SlotID = StrongID<SlotTag>;
	struct ProfileTag {}; using ProfileID = StrongID<ProfileTag>;
	struct FSMTag {}; using FSMID = StrongID<FSMTag>;
	struct AxisTag {}; using AxisID = StrongID<AxisTag>;

	inline constexpr StateID	kInvalidState {};
	inline constexpr CondID		kInvalidCond {};
	inline constexpr SlotID		kInvalidSlot {};
	inline constexpr ProfileID	kInvalidProfile {};
	inline constexpr FSMID		kInvalidFSM {};
	inline constexpr AxisID		kInvalidAxis {};
	
	struct Transition
	{
		StateID from;
		StateID to;
		SlotID slot;
		std::uint8_t prio;
	};

	struct Profile
	{
		ProfileID id{};

		ProfileID extends = kInvalidProfile;

		std::vector<CondID> condOfSlot;// FSMローカルのslots順
	};

	struct FSM
	{
		FSMID fsm;		
		std::vector<StateID> states;	// subset
		std::vector<CondID> conds;		// subset
		std::vector<SlotID> slots;		// subset
		
		std::vector<Transition> transitions;
		std::vector<Profile> profs;
	};

	// 
	struct Axis
	{
		AxisID id;
		std::vector<StateID> states;
		std::vector<CondID> conds;
		std::vector<SlotID> slots;
		std::vector<ProfileID> profiles;
		std::vector<FSM> fsms;
	};

	struct TransitionEdge 
	{ 
		std::uint32_t toIdx;
		uint8_t prio; 
	};

	struct CanonicalFSM
	{
		FSMID id{};
		std::uint32_t numStates = 0;
		std::uint32_t numSlots = 0;
		std::uint32_t numProfiles = 0;

		// i番目の(state,slot)のedges開始位置 (ofs[i+1]-ofs[i]:出次数)
		std::vector<uint32_t> ofs;			// size = numStates * numSlots + 1 
		// 
		std::vector<TransitionEdge> edges;	// ソート後のtoのindex

		std::vector<CondID> condOf;			// size = numProfiles * numSlots

		// ローカル→グローバルの写像
		std::vector<StateID> local2GlobalState;
		std::vector<SlotID> local2GlobalSlot;
		std::vector<ProfileID> local2GlobalProfile;

		std::uint16_t version = 1;
	};

	struct CanonicalAxis
	{
		AxisID axis;
		std::vector<CanonicalFSM> fsms;
	};

	struct FSMCatalog
	{    
		std::vector<CanonicalAxis> axes;
	};
}