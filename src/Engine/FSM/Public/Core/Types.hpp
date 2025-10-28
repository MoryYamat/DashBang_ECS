#pragma once

// 記号的な状態遷移のすべての体系を行うシステム(エンジン)
// データ構造とインターフェース
// 遷移システムを提供する

#include "Engine/ECS/Public/Entity.hpp"

#include <cstdint>
#include <span>
#include <vector>
#include <string>

namespace Engine::FSM::Core
{
	// 全FSM空間の中で一意
	struct AxisID
	{
		std::uint16_t id;// 仮
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

	struct TransitionView
	{
		std::span<const Transition> outgoing(StateID s) const;// 遷移集合を検索し抽出
	};

	// 
	struct EnvSnapshot
	{

	};

	struct CondTable
	{
		bool eval(CondID id, const EnvSnapshot& env, const EvalCtx& ctx) const;
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
		Reason reason{ 0 };
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
}