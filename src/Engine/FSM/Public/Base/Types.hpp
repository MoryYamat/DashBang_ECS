#pragma once

#include <cstdint>
#include <vector>
#include <utility>
#include <string>
#include <string_view>

#include <span>

#include <unordered_map>
#include <cassert>

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
		std::string axisName;

		// 軸宇宙（辞書順採番に使った最終順序）
		std::vector<std::string> stateOrder;
		std::vector<std::string> condOrder;
		std::vector<std::string> slotOrder;
		std::vector<std::string> profileOrder;

		std::vector<CanonicalFSM> fsms;
	};

	struct FSMCatalog
	{    
		std::vector<CanonicalAxis> axes;
	};


	// Cond
	struct EvalCtx
	{
		std::uint32_t entity;
	};

	struct EnvSnapshot
	{
		// 事前にまとめて計算した値．移動入力、時刻など
		virtual ~EnvSnapshot() = default;
	};

	struct CondTable
	{
		using Fn = bool(*)(const EnvSnapshot&, const EvalCtx&);
		std::vector<Fn> fns;

		void init(std::size_t numConds)
		{
			fns.assign(numConds, nullptr);
		}

		void bind(CondID id, Fn fn)
		{
			if (!id.valid()) return;
			if (id.v >= fns.size()) fns.resize(id.v + 1, nullptr);
			fns[id.v] = fn;
		}

		bool eval(CondID c, const EnvSnapshot& env, const EvalCtx& ctx) const
		{
			const auto i = c.v;
			if (!c.valid() || i >= fns.size()) return false;
			Fn fn = fns[i];
			return fn ? fn(env, ctx) : false;
		}
	};

	struct AxisRuntime
	{
		AxisID id;
		CondTable conds;
		const CanonicalAxis* canon = nullptr;// 読み取り用
	};

	struct Decision
	{
		std::uint32_t from;	// local state idx
		std::uint32_t to;	// local state idx (unchanged => from)
		bool changed;
	};

	Decision DecideNext_BySingleSlot
	(
		const CanonicalFSM& f,
		const AxisRuntime& ax,
		std::uint32_t fromLocal,
		std::uint32_t profileLocal,
		std::uint32_t slotLocal,
		const EnvSnapshot& env,
		const EvalCtx& ctx
	);

	Decision DecideNext_Slots
	(
		const CanonicalFSM& f,
		const AxisRuntime& ax,
		std::uint32_t fromLocal,
		std::uint32_t profileLocal,
		std::span<const std::uint32_t> slots,
		const EnvSnapshot& env,
		const EvalCtx& ctx
	);

	struct NamedCondBinding
	{
		std::string_view name;
		CondTable::Fn fn;
	};

	struct AxisRuntimeDB
	{
		std::unordered_map<std::string, AxisRuntime> axes;
		AxisRuntime* get(const std::string& name)
		{
			auto it = axes.find(name);
			return it != axes.end() ? &it->second : nullptr;
		}

		AxisRuntime& ensure(const CanonicalAxis& ax)
		{
			auto [it, ok] = axes.emplace(ax.axisName, AxisRuntime{});
			it->second.id = ax.axis;
			it->second.canon = &ax;
			return it->second;
		}
	};
}