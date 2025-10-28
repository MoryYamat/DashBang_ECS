#pragma once

// 記号的な状態遷移のすべての体系を行うシステム(エンジン)
// データ構造とインターフェース
// 遷移システムを提供する

#include "Engine/ECS/Public/Entity.hpp"

#include <cstdint>
#include <span>
#include <vector>
#include <string>

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
}