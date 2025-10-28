#pragma once

#include "Engine/ECS/Public/Entity.hpp"

#include <cstdint>

#include <vector>
#include <utility>
#include <span>

namespace Game::Character::FSM::Movement
{
	struct StateID
	{
		uint32_t id;
	};

	struct CondID
	{
		uint32_t id;
	};

	// Movement Transition評価に必要な値を集約
	struct MovementEnvSnapShot
	{

	};


	struct Transition
	{
		StateID to;
		CondID cond;
		int16_t prio;
	};

	struct TransitionView
	{

		std::span<const Transition> outgoing(StateID from) const;// 遷移集合を検索し抽出
	};

	struct CondTable
	{
		bool eval(CondID id, const MovementEnvSnapShot& env, Engine::ECS::Core::Entity e) const;
	};

	struct StateMeta
	{
		int priority(StateID s) const;
		bool interruptible(StateID s)const;
	};

	struct Decision
	{
		StateID to;
		bool changed;
	};

	template<typename Env, typename TV, typename CT, typename SM>
	Decision decide_next(
		Engine::ECS::Core::Entity e,
		StateID from,
		const TV& transitions,
		const CT& conds,
		const SM& meta,
		const Env& env
	);
}


// csr :Compressed Sparse Row