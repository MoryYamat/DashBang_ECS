#include "CCFSMSystem.hpp"

#include "Engine/Time/Private/WorldClock.hpp"

#include "Game/Character/Private/Control/CC/Component/CCAntiChainComponent.hpp"

#include "Game/Character/Private/FSM/CC/Database/CCFSMDatabase.hpp"

#include "Game/Character/Private/FSM/CC/StateModel/CCStateComponent.hpp"
#include "Game/Character/Private/FSM/CC/StateModel/CCFSMTransitionRequestComponent.hpp"
#include "Game/Character/Private/FSM/CC/StateModel/CCFSMContext.hpp"

namespace Game::Character::FSM::CC::System
{
	void CCFSMSysmtem(Engine::ECS::EntityMgr& ecs)
	{
		using namespace Engine::Time;
		using namespace Game::Character::Control::CC::Component;
		using namespace Game::Character::FSM::CC::Database;
		using namespace Game::Character::FSM::CC::StateModel;

		const auto& clock = ecs.getResource<WorldClockData>();

		const auto& db = ecs.getResource<CCFSMDatabase>();
		if (!db.Has("basic")) return;
		const auto& def = db.Get("basic"); // TODO: 将来entity事に違う定義を持てるように

		for (auto e : ecs.view<
			CCStateComponent,
			CCFSMTransitionRequestComponent>())
		{
			auto& state = ecs.get<CCStateComponent>(e);
			auto& reqs = ecs.get<CCFSMTransitionRequestComponent>(e);

			// TODO: CCFSMContextの構築方法を汎用的にする(現状はAntiChainのみになっている)
			CCFSMContext ctx{};
			const bool inCC = (state.current != StateTag::NONE) && (state.current != StateTag::IMMUNE);

			if (!inCC)
			{
				// 必要なら NONE/IMMUNE 用の遷移を評価（今回は無いからスキップでOK）
				ctx.currentCC.reset();
				ctx.ccEnteredAt = 0.0f;
				ctx.ccDuration = 0.0f;
			}
			else
			{
				ctx.currentCC = state.current;
				ctx.ccEnteredAt = state.enteredAt;
				const float dur = clock.now - state.enteredAt;
				ctx.ccDuration = (dur >= 0.0f) ? dur : 0.0f;
			}


			for (const auto& trans : def.transitions)
			{
				// from条件があるならチェック(nullopt = すべての状態から許容)
				if (trans.from.has_value() && state.current != trans.from.value()) continue;
				if (!trans.condition->evaluate(ctx)) continue;

				if (reqs.hasExactRequest(trans.to, 0)) continue;

				reqs.requests.push_back(
					{
						.requestedTo = trans.to,
						.priority = 0// 現在は固定・テスト用
					});

				std::cout << "[CCFSMSystem] Requesting transition: ->  "
					<< " -> " << trans.to.name() << std::endl;

				// break;
			}
		}
	}
}