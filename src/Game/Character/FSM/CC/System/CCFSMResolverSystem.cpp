#include "CCFSMResolverSystem.hpp"

#include "Game/Character/FSM/CC/StateModel/CCStateComponent.hpp"
#include "Game/Character/FSM/CC/StateModel/CCFSMTransitionRequestComponent.hpp"

#include "Game/Character/FSM/CC/Effect/StateScoped/CCStateEffectExecutionRecordComponent.hpp"

#include "Game/Character/FSM/CC/Database/CCFSMDatabase.hpp"

#include <iostream>

// FIXME: ResolverとStateScopedの副作用は分離したほうがよい
namespace Game::Character::FSM::CC::System
{
	using namespace Game::Character::FSM::CC::Database;
	using namespace Game::Character::FSM::CC::StateModel;

	void CCFSMResolverSystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
	{
		auto& db = ecs.getResource<CCFSMDatabase>();
		if (db.Has("basic")) return;
		auto& def = db.Get("basic");

		for (auto e : ecs.view<
			CCStateComponent,
			CCFSMTransitionRequestComponent
		>())
		{
			auto& state = ecs.get<CCStateComponent>(e);
			auto& reqs = ecs.get<CCFSMTransitionRequestComponent>(e);


			if (reqs.requests.empty()) continue;

			const CCFSMTransitionRequest* bestRequest = nullptr;

			for (const auto& req : reqs.requests)
			{
				if (!req.requestedTo.has_value()) continue;
				if (!bestRequest || req.priority > bestRequest->priority)
				{
					bestRequest = &req;
				}
			}

			if (bestRequest)
			{
				//auto best = std::max_element(
				//	reqs.requests.begin(),
				//	reqs.requests.end(),
				//	[](const auto& a, const auto& b) {
				//		return a.priority < b.priority;
				//	}
				//);

				// 状態の更新
				const std::type_index fromState = state.current;
				const std::type_index toState = bestRequest->requestedTo.value();

				state.previous = fromState;
				state.current = toState;

				std::cout << "[CCFSMResolverSystem] Transition applied: "
					<< fromState.name() << " -> "
					<< toState.name() << std::endl;

				CCFSMContext ctx;

				for (const auto& hook : def.hooks)
				{
					tryTriggerEffect(ecs, hook, e, def, ctx, state.current, state.previous);
				}
			}


			// リクエストクリア
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