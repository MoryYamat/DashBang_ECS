#include "MovementFSMResolverSystem.hpp"

#include "Game/Character/FSM/Movement/StateModel/MovementFSMContext.hpp"

#include "Game/Character/Control/Movement/Component/Intent/MovementIntentComponent.h"

#include "Game/Character/FSM/Movement/StateModel/MovementFSMTransitionRequestComponent.hpp"


// FIXME: ResolverとStateScopedの副作用は分離したほうがよい
void Game::Character::FSM::Movement::System::MovementFSMResolverSystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Engine::ECS::Component::Logic2D;
	using namespace Game::Character::Intent;
	using namespace Game::Character::FSM::Movement;
	using namespace Game::Character::FSM::Movement::Effect;
	using namespace Game::Character::FSM::Movement::Database;
	using namespace Game::Character::FSM::Movement::StateModel;

	const auto& db = ecs.getResource<MovementFSMDatabase>();
	// const auto& def = db.Get("basic");// TODO: 将来 entityごとに切り替え可能 // 現在未使用

	for (eNsECS::Entity e : ecs.view<
		MovementStateComponent,
		MovementFSMTransitionRequestComponent,
		MovementFSMLeaseComponent
	>()
		)
	{
		auto& state = ecs.get<MovementStateComponent>(e);
		auto& reqs = ecs.get<MovementFSMTransitionRequestComponent>(e);
		auto& lease = ecs.get<MovementFSMLeaseComponent>(e);

		if (tryApplyForcedTransition(ecs, e, state, lease, db))
		{
			reqs.requests.clear();
			continue;
		}


		if (reqs.requests.empty()) continue;


		// std::cout << "here\n";

		// 数値が大きいほど優先度が高い
		std::sort(reqs.requests.begin(), reqs.requests.end(),
			[](const auto& a, const auto& b) { return a.priority > b.priority; });

		// 最初のリクエストを受諾（今後，条件付き受諾も可能に）
		const auto& request = reqs.requests.front();

		//if (state.current == request.requestedTo)
		//{
		//	reqs.requests.clear();
		//	continue;
		//}


		// TODO: 遷移処理のAPI化
		//std::type_index previous = state.current;
		//state.previous = previous;
		//state.current = request.requestedTo;

		if (applyStateUpdate(state, request.requestedTo))
		{
			std::cout << "[MovementFSMTransitionSystem]: Transition accepted: "
				<< state.previous.name() << " → " << request.requestedTo.name() << "\n";

			runMovementEffects(ecs, e, state, db);
		}




		// ============ API 化
		// Effect実行（Triggerに合致するものを探す）
		//MovementFSMContext ctx;
		//if (ecs.hasComponent<MovementIntentComponent>(e))
		//{
		//	const auto& intent = ecs.get<MovementIntentComponent>(e);
		//	ctx.intentActive = intent.isActive;
		//	ctx.direction = intent.direction;
		//}

		//std::cout << "[FSMResolverSystem] ctx.direction = ("
		//	<< ctx.direction.x << ", " << ctx.direction.y << ")\n";

		//for (const auto& hook : def.effectHooks)
		//{
		//	if (hook.trigger->evaluate(ctx, state.current, state.previous))
		//	{
		//		hook.effect->apply(ecs, e, ctx);
		//	}
		//}

		// ===============


		// リクエストを消去
		reqs.requests.clear();
	}
}

namespace Game::Character::FSM::Movement::System
{
	using namespace Game::Character::Intent;

	bool MovementFSMResolverSystem::tryApplyForcedTransition
	(
		Engine::ECS::EntityMgr& ecs,
		const Engine::ECS::Entity e,
		Game::Character::FSM::Movement::MovementStateComponent& state,
		const Game::Character::FSM::Movement::StateModel::MovementFSMLeaseComponent& lease,
		const Game::Character::FSM::Movement::Database::MovementFSMDatabase& db
	)
	{
		using IM = Game::Character::FSM::Interference::Core::Data::InterferenceMode;
		if (lease.mode != IM::ForceTransition) return false;
		if (!lease.forcedState.has_value()) return false;

		const auto target = *lease.forcedState;

		if (applyStateUpdate(state,target))
		{
			runMovementEffects(ecs, e, state, db);
			// std::type_index previous = state.current;
			// state.previous = previous;
			// state.current = target;

		}

		return true;
	}


	bool MovementFSMResolverSystem::applyStateUpdate(
		Game::Character::FSM::Movement::MovementStateComponent& state,
		std::type_index to
	)
	{
		if (state.current == to) return false;
		state.previous = state.current;
		state.current = to;
		return true;
	}

	void MovementFSMResolverSystem::runMovementEffects
	(
		Engine::ECS::EntityMgr& ecs,
		const Engine::ECS::Entity e,
		Game::Character::FSM::Movement::MovementStateComponent& state,
		const Game::Character::FSM::Movement::Database::MovementFSMDatabase& db
	)
	{
		const auto& def = db.Get("basic");// TODO: 将来 entityごとに切り替え可能

		// context 作成
		MovementFSMContext ctx;
		if (ecs.hasComponent<MovementIntentComponent>(e))
		{
			const auto& intent = ecs.get<MovementIntentComponent>(e);
			ctx.intentActive = intent.isActive;
			ctx.direction = intent.direction;
		}

		std::cout << "[FSMResolverSystem] ctx.direction = ("
			<< ctx.direction.x << ", " << ctx.direction.y << ")\n";


		// 副作用実行
		for (const auto& hook : def.effectHooks)
		{
			if (hook.trigger->evaluate(ctx, state.current, state.previous))
			{
				hook.effect->apply(ecs, e, ctx);
			}
		}

	}
}