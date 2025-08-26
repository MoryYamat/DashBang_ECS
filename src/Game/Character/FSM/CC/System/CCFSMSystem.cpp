#include "CCFSMSystem.hpp"

#include "Game/Character/FSM/CC/Database/CCFSMDatabase.hpp"

#include "Game/Character/FSM/CC/StateModel/CCStateComponent.hpp"
#include "Game/Character/FSM/CC/StateModel/CCFSMTransitionRequestComponent.hpp"
#include "Game/Character/FSM/CC/StateModel/CCFSMContext.hpp"

namespace Game::Character::FSM::CC::System
{
	void CCFSMSysmtem(eNsECS::EntityMgr& ecs)
	{
		using namespace Game::Character::FSM::CC::Database;
		using namespace Game::Character::FSM::CC::StateModel;
		const auto& db = ecs.getResource<CCFSMDatabase>();
		const auto& def = db.Get("basic"); // TODO: 将来entity事に違う定義を持てるように

		for (auto e : ecs.view<
			CCStateComponent,
			CCFSMTransitionRequestComponent>())
		{
			auto& state = ecs.get<CCStateComponent>(e);
			auto& reqs = ecs.get<CCFSMTransitionRequestComponent>(e);

			CCFSMContext ctx;

			for (auto trans : def.transitions)
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

				std::cout << "[MovementFSMResolverSystem] Requesting transition: "
					<< " -> " << trans.to.name() << std::endl;

				// break;
			}
		}
	}
}