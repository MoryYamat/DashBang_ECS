#include "MovementFSMResolverSystem.hpp"

#include "Game/Character/FSM/Movement/Database/MovementFSMDatabase.hpp"

#include "Game/Character/Control/Movement/Component/Intent/MovementIntentComponent.h"

#include "Game/Character/FSM/Movement/StateModel/MovementStateComponent.hpp"

#include "Game/Character/FSM/Movement/StateModel/MovementFSMTransitionRequestComponent.hpp"

// FIXME: ResolverとStateScopedの副作用は分離したほうがよい
void Game::Character::FSM::Movement::System::MovementFSMResolverSystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Engine::ECS::Component::Logic2D;
	using namespace Game::Character::Intent;
	using namespace Game::Character::FSM::Movement;
	using namespace Game::Character::FSM::Movement::Effect;
	using namespace Game::Character::FSM::Movement::Database;

	const auto& db = ecs.getResource<MovementFSMDatabase>();
	const auto& def = db.Get("basic");// TODO: 将来 entityごとに切り替え可能


	for (eNsECS::Entity e : ecs.view<
		MovementStateComponent,
		MovementFSMTransitionRequestComponent>()
		)
	{
		auto& state = ecs.get<MovementStateComponent>(e);
		auto& reqs = ecs.get<MovementFSMTransitionRequestComponent>(e);

		if (reqs.requests.empty()) continue;


		// std::cout << "here\n";

		// 数値が大きいほど優先度が高い
		std::sort(reqs.requests.begin(), reqs.requests.end(),
			[](const auto& a, const auto& b) { return a.priority < b.priority; });

		// 最初のリクエストを受諾（今後，条件付き受諾も可能に）
		const auto& request = reqs.requests.front();

		//if (state.current == request.requestedTo)
		//{
		//	reqs.requests.clear();
		//	continue;
		//}

		std::type_index previous = state.current;
		state.previous = previous;
		state.current = request.requestedTo;

		std::cout << "[MovementFSMTransitionSystem]: Transition accepted: "
			<< previous.name() << " → " << request.requestedTo.name() << "\n";

		// Effect実行（Triggerに合致するものを探す）
		MovementFSMContext ctx;
		if (ecs.hasComponent<MovementIntentComponent>(e))
		{
			const auto& intent = ecs.get<MovementIntentComponent>(e);
			ctx.intentActive = intent.isActive;
			ctx.direction = intent.direction;
		}

		std::cout << "[FSMResolverSystem] ctx.direction = ("
			<< ctx.direction.x << ", " << ctx.direction.y << ")\n";


		for (const auto& hook : def.effectHooks)
		{
			if (hook.trigger->evaluate(ctx, state.current, previous))
			{
				hook.effect->apply(ecs, e, ctx);
			}
		}

		// リクエストを消去
		reqs.requests.clear();
	}
}