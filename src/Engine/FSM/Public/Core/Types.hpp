#pragma once

// 記号的な状態遷移のすべての体系を行うシステム(エンジン)
// データ構造とインターフェース
// 遷移システムを提供する

#include "Engine/ECS/Public/Entity.hpp"

#include <unordered_map>
#include <cstdint>
#include <span>
#include <vector>
#include <string>
#include <utility>
#include <cassert>

namespace Engine::FSM::Core
{
	// 全FSM空間の中で一意
	struct AxisID
	{
		std::uint32_t id;// 仮
	};

	// 全FSM空間の中で一意
	struct StateID
	{
		std::uint32_t id;// 仮
	};

	// 各FSM空間の中で一意
	struct CondID
	{
		std::uint32_t id;// 仮
	};

	using CondProfileID = std::uint32_t;

	struct Transition
	{
		StateID to;
		CondID cond;
		std::uint8_t prio;
		std::uint8_t flags{ 0 };
		std::uint16_t pad{ 0 };
	};


	struct EntityKey
	{
		uintptr_t raw;
	};

	struct EvalCtx
	{
		EntityKey entity;
	};

	//struct TransitionView
	//{
	//	std::span<const Transition> outgoing(StateID s) const;// 遷移集合を検索し抽出
	//};

	// 
	struct EnvSnapshot
	{

	};

	struct CondTable
	{
		// TODO 事前コンパイル方式: オーサリング時に式→バイトコードへコンパイル(実行時は配列を順に読むだけ)
		using Fn = bool(*)(const EnvSnapshot&, const EvalCtx&);
		std::vector<Fn> fns;// index == CondID.id

		void ensureSize(std::size_t n)
		{
			if (fns.size() < n) fns.resize(n, nullptr);
		}

		void set(CondID id, Fn fn)
		{
			ensureSize(id.id + 1);
			fns[id.id] = fn;
		}

		bool eval(CondID id, const EnvSnapshot& env, const EvalCtx& ctx) const
		{
			const auto i = id.id;
			assert(i < fns.size());
			auto* fn = (i < fns.size()) ? fns[i] : nullptr;
			return fn ? fn(env, ctx) : false;
		}
	};

	constexpr std::uint8_t kMaxPrio = 255;

	enum class Reason : std::uint8_t
	{
		None = 0,
		FirstTrue = 1,
		HigherPrio = 2
	};

	struct [[nodiscard]] Decision
	{
		StateID to;
		CondID cond{};
		bool changed{ false };
		Reason reason{ Reason::None };
	};

	struct CanonicalAxis
	{
		AxisID axis;
		std::vector<StateID> stateIDs;
		std::vector<Transition> edges;	// 遷移の平坦化配列
		std::vector<uint32_t> headIndex;// 状態iの遷移リストの開始位置

		std::vector<std::string> stateNames;
		std::vector<std::string> condNames;
		std::string axisName;
		std::uint32_t schemaVersion;
	};


	struct TransitionView_CSR
	{
		std::span<const Transition> edges;
		std::span<const std::uint32_t> headIndex;// [stateCount + 1]

		[[nodiscard]] std::span<const Transition> outgoing(StateID s) const noexcept
		{
			assert(s.id + 1 < headIndex.size());
			const auto b = headIndex[s.id];
			const auto e = headIndex[s.id + 1];
			return { edges.data() + b, static_cast<size_t>(e - b) };
		}
	};

	[[nodiscard]] inline TransitionView_CSR MakeTransitionView(const CanonicalAxis& ca) noexcept
	{
		return TransitionView_CSR{
			std::span<const Transition>(ca.edges.data(), ca.edges.size()),
			std::span<const std::uint32_t>(ca.headIndex.data(), ca.headIndex.size())
		};
	}

	[[nodiscard]] Decision TransitionSelector
	(
		StateID from,
		const TransitionView_CSR& tv,
		const CondTable& ct,
		const EnvSnapshot& env,
		const EvalCtx& ctx
	);

	// staging helper
	struct CondTableStaging
	{
		std::vector<std::pair<std::string, CondTable::Fn>> byName;

		void add(std::string name, CondTable::Fn fn)
		{
			byName.emplace_back(std::move(name), fn);
		}
	};

	struct AxisProfileKey
	{
		std::string axisName;
		CondProfileID profileId{};
		bool operator == (const AxisProfileKey& o) const noexcept
		{
			return profileId == o.profileId && axisName == o.axisName;
		}
	};

	struct AxisProfileKeyHash
	{
		size_t operator()(const AxisProfileKey& k) const noexcept
		{
			return std::hash<std::string>{}(k.axisName) ^ (std::hash<std::uint32_t>{}(k.profileId) << 1);
		}
	};

	using CondStagesPerAxisProfile
		= std::unordered_map<AxisProfileKey, CondTableStaging, AxisProfileKeyHash>;


	// build済みのCanonicalAxisにステージで貯めた名前付き関数群を整合させて流し込む
	inline bool FinalizeCondTable
	(
		const CanonicalAxis& ca,
		const CondTableStaging& stage,
		CondTable& out
	)
	{
		bool ok = true;
		//
		for (const auto& [name, fn] : stage.byName)
		{
			const auto& v = ca.condNames;
			auto it = std::lower_bound(v.begin(), v.end(), name);
			//
			if (it == v.end() || *it != name)
			{
				// assert(it != v.end() && *it == name && "Cond name not found in CanonicalAxis");
				ok = false;
				continue;
			}
			CondID id{ static_cast<std::uint32_t>(it - v.begin()) };
			out.set(id, fn);
		}

		return ok;
	}




	// 結果
	struct FSMCatalog
	{
		std::vector<CanonicalAxis> axes;
	};

	struct FSMCondTables
	{
		std::vector<CondTable> byAxis;// size == FSMCatalog.axes.size();
	};

	struct FSMCondProfiles
	{
		std::vector<std::unordered_map<CondProfileID, CondTable>> byAxis;
	};



	inline const CondTable& ResolveCondTableForAxisProfile
	(
		const FSMCondProfiles& profs,
		AxisID axis,
		CondProfileID profileId
	)
	{
		const auto& perAxis = profs.byAxis[axis.id];
		if (auto it = perAxis.find(profileId); it != perAxis.end())
			return it->second;

		return perAxis.at(0);
	}
}