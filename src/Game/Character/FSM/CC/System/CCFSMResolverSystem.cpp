#include "CCFSMResolverSystem.hpp"

#include "Game/Character/Control/CC/Policy/CCAntiChainPolicyDatabase.hpp"
#include "Game/Character/Control/CC/Component/CCAntiChainComponent.hpp"

#include "Game/Character/FSM/CC/StateModel/CCStateComponent.hpp"
#include "Game/Character/FSM/CC/StateModel/CCFSMTransitionRequestComponent.hpp"

#include "Game/Character/FSM/CC/Effect/StateScoped/CCStateEffectExecutionRecordComponent.hpp"

#include "Game/Character/FSM/CC/Database/CCFSMDatabase.hpp"

// API
// Internal
#include "Game/Character/FSM/CC/API/Internal/UpdateCCFSMContext.hpp"

// clock
#include "Engine/Time/WorldClock.hpp"

#include <iostream>

// FIXME: ResolverとStateScopedの副作用は分離したほうがよい
namespace Game::Character::FSM::CC::System
{
	using namespace Engine::Time;
	using namespace Game::Character::FSM::CC::Database;
	using namespace Game::Character::FSM::CC::StateModel;

	using namespace Game::Character::Control::CC::Policy;
	using namespace Game::Character::Control::CC::Component;

	void CCFSMResolverSystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
	{
		const auto& clock = ecs.getResource<WorldClockData>();

		// std::cout << "[now] clock = " << clock.now << std::endl;


		const auto& db = ecs.getResource<CCFSMDatabase>();
		if (!db.Has("basic")) return;
		const auto& def = db.Get("basic");// 現在固定

		const auto& pdb = ecs.getResource<CCAntiChainPolicyDatabase>();
		const auto& policy = pdb.ResolveForFSM("basic");// ひとまず固定

		for (auto e : ecs.view<
			CCStateComponent,
			CCFSMTransitionRequestComponent,
			CCAntiChainComponent
		>())
		{
			auto& state = ecs.get<CCStateComponent>(e);
			auto& reqs = ecs.get<CCFSMTransitionRequestComponent>(e);
			auto& anti = ecs.get<CCAntiChainComponent>(e);

			// 先頭で以前の状態を更新
			// state.previous = state.current;
			state.beginFrameSnapshot();

			if (reqs.requests.empty()) continue;

			// 現在の状態が CC中
			const auto isCC = [&](const std::type_index& t)
				{
					return t != StateTag::NONE && t != StateTag::IMMUNE;
				};


			// Debug：正常動作
			// このフレームで適用してよいリクエストか判定
			auto admissible = [&](const CCFSMTransitionRequest& r) -> bool {
				if (!r.requestedTo) return false;
				const auto& to = r.requestedTo.value();
				// IMMUNE 中は NONE 以外を拒否
				// IMMUNE 中はNONEのみを通す
				if (state.current == StateTag::IMMUNE) return to == StateTag::NONE;

				// std::cout << "[CCFSMResolverSystem.cpp] anti.count = " << anti.count << "\n";
				// 閾値到達後 CC中なら NONE || IMMUNE 以外は無視
				if (anti.immuneArmed && isCC(state.current))
					return to == StateTag::IMMUNE || to == StateTag::NONE;
				
				return true;
				};




			// 一発で最優先を選ぶ（同 priority は挿入順優先＝安定）
			const CCFSMTransitionRequest* best = nullptr;
			for (const auto& r : reqs.requests) {
				if (!admissible(r)) continue;
				if (!best || r.priority > best->priority) best = &r;
			}

			if (!best) { reqs.requests.clear(); continue; }

			// =============
			// このあたりで anti.count に基づいてcount>=threshold以上の場合，CCリクエストをブロックしなければならない．

			// 
			// ==============

			// 仕様: 状態が更新される瞬間しか current/ previousは更新されない
			// 

			// 状態の更新
			// const std::type_index fromState = state.current;
			const std::type_index toState = best->requestedTo.value();

			//state.previous = fromState;
			//state.current = toState;

			state.applyTransition(toState, clock.now);


			// 何らかのCCを受けた瞬間の時刻を保存
			if (state.current != StateTag::NONE && state.current != StateTag::IMMUNE)
			{
				state.enteredAt = clock.now;
			}

			std::cout << "[CCFSMResolverSystem] Transition applied: "
				<< state.previous.name() << " -> " << toState.name() 
				<< " /applied At = " << state.applied.appliedAt 
				<< std::endl;


			// TODO: antiChain以外のIMMUNE源からのctx更新もできるように拡張
			CCFSMContext ctx{};
			if (state.current != StateTag::NONE && state.current != StateTag::IMMUNE)
			{
				ctx.currentCC = state.current;
				ctx.ccEnteredAt = state.enteredAt;
				ctx.ccDuration = clock.now - state.enteredAt;
			}


			// TODO: API化 
			//ctx.chainCount = anti.count;
			//ctx.immune = anti.immune;
			//ctx.immuneEndsAt = anti.immuneUntil;
			//ctx.chainWindowStart = anti.windowStart;
			Game::Character::FSM::CC::API::updateContextFromAnti(ctx, anti);
			// TODO: antiChain以外のIMMUNE源からのctx更新もできるように拡張

			for (const auto& hook : def.hooks)
			{
				// TODO: Executeの実動作の実装
				tryTriggerEffect(ecs, hook, e, def, ctx, state.current, state.previous);
			}


			reqs.requests.clear();
		}

	}

	// record へ Effect関数オブジェクトの型をhash化して登録する
	void CCFSMResolverSystem::tryTriggerEffect(
		eNsECS::EntityMgr& ecs,
		const CCFSMStateEffectHook& hook,
		const eNsECS::Entity entity,
		const CCFSMDefinition& def,
		const CCFSMContext& ctx,
		const std::type_index& current,
		const std::type_index& previous)
	{
		if (!ecs.hasComponent<CCStateEffectExecutionRecordComponent>(entity))
		{
			ecs.addComponent(entity, CCStateEffectExecutionRecordComponent{});
		}

		auto& record = ecs.get<CCStateEffectExecutionRecordComponent>(entity);

		std::size_t hash = std::type_index(typeid(*hook.handler)).hash_code();

		if (hook.trigger->evaluate(ctx, current, previous) &&
			!record.hasExecuted(hash))
		{
			hook.handler->execute(entity, ctx);
			record.markExecuted(hash);
		}

		std::cout << "[tryTriggerEffect(CCFSM)] effectHash: " << hash << "\n";
		std::cout << "[tryTriggerEffect(CCFSM)] hasExecuted: " << record.hasExecuted(hash) << "\n";
		std::cout << "[tryTriggerEffect(CCFSM)] trigger.evaluate: " << hook.trigger->evaluate(ctx,current, previous) << "\n";

	}

	// FIXME:この汎用構造の中で`EffectExecutionRecordComponent`のリセットを他関数の定義によって実行するのは気持ちが悪い
	// FIXME:デフォルトのリセット関数としての機能で`EffectExecutionRecordComponent`をリセットすることにするならよいかも？
	// FIXME:ともかく情報の構造から実装の依存関係を整理する必要がある
	// record の リセット
	void CCFSMResolverSystem::tryTriggerReset(
		eNsECS::EntityMgr& ecs,
		const CCFSMStateEffectHook& hook,
		const eNsECS::Entity entity,
		const CCFSMDefinition& def,
		const CCFSMContext& ctx,
		const std::type_index& current,
		const std::type_index& previous)
	{

	}
}