#include "MovementFSMInterferenceResolver.hpp"

#include "Engine/Time/WorldClock.hpp"

#include "Game/Character/FSM/Interference/Core/Utils/ResolverUtil.hpp"


#include "Game/Character/FSM/Movement/MovementStateTags.hpp"

#include <iostream>

namespace Game::Character::FSM::Movement::Interference
{
	using namespace Engine::Time;

	using namespace Engine::ECS;
	using namespace Game::Character::FSM::Interference::Core::Data;
	using namespace Game::Character::FSM::Movement::StateModel;
	using namespace Game::Character::FSM::Movement;

	void MovementFSMInterferenceResolver::Update(EntityMgr& ecs)
	{
		const auto& clock = worldClock(ecs);

		for (auto e : ecs.view<MovementFSMInterferenceRequestComponent, MovementStateComponent, MovementFSMLeaseComponent>())
		{
			auto& request = ecs.get<MovementFSMInterferenceRequestComponent>(e);
			auto& state = ecs.get<MovementStateComponent>(e);
			auto& lease = ecs.get<MovementFSMLeaseComponent>(e);


			if (auto selected = computeHighestPriorityRequest(ecs, e, request, state, lease))
			{
				acceptInterference(ecs, e, *selected, lease, clock.now);
			}

			// FIX:
			// CC時間の上書きができていない．
			// おそらく LeaseCompの上書きができていないからだと思われる
			//

			updateInterference(ecs, e, lease, clock.now ,clock.dt);// 状態は変えない (状態更新はFSMResovlerで)


			request.requests.clear();
		}
	}



	const FSMInterferenceRequest* MovementFSMInterferenceResolver::computeHighestPriorityRequest(
		EntityMgr& ecs,
		Entity e,
		const MovementFSMInterferenceRequestComponent& requestComp,
		MovementStateComponent& state,
		MovementFSMLeaseComponent& lease)
	{

		using namespace Game::Character::FSM::Interference::Core::Util;
		using namespace Game::Character::FSM::Movement;


		// 干渉リクエストの複数適用を許容する仕様の場合以下が必要
		// まだ使用を決めるのが難しいので保留
		// TODO: 複数リクエストの異なるモードの干渉を許可するかどうか
		// TODO: 一つの干渉リソースが複数のモードを指定することができるようにするかどうか(bitflag化)
		// TODO: モードの詳細も考える
		//std::vector<const FSMInterferenceRequest*> validRequests;

		//for (const auto& req : requestComp.requests)
		//{
		//	if (shouldApply(req, AxisTag::MovementAxis) && isControlAllowed(req, lease.severity))
		//	{
		//		validRequests.push_back(&req);
		//	}
		//}

		//// 最大優先度を計算
		//auto maxSeverity = std::max_element(validRequests.begin(), validRequests.end(),
		//	[](const auto* a, const auto* b) {
		//		return a->severity < b->severity;
		//	});

		//if (maxSeverity == validRequests.end()) return;


		const auto targetAxis = AxisTag::MovementAxis;

		const FSMInterferenceRequest* selected = nullptr;

		for (const auto& req : requestComp.requests)
		{
			// 有効なrequestか確認(durationSec>0 && targetAxis == AxisTag::MovementAxis)
			if (!shouldApply(req, targetAxis)) continue;
			// forcedStateがなければ無視
			if (!req.forcedState.has_value()) continue;
			
			// accept時に比較するように変更
			// // 現在のlease.severityより小さいなら無視
			// if (req.severity < lease.severity) continue;

			// 
			if (!selected || req.severity > selected->severity)
				selected = &req;
		}

		if (!selected) return nullptr;
		return selected;

		// 強制遷移処理
		// applyForcedTransition(lease, state, *selected);

		//lease.severity = selected->severity;
		//lease.remainingDurationSec = selected->durationSec;

		//state.current = *selected->forcedState;
	}

	void MovementFSMInterferenceResolver::acceptInterference(
		Engine::ECS::EntityMgr& ecs,
		Engine::ECS::Entity e,
		const FSMInterferenceRequest& req,
		Game::Character::FSM::Movement::StateModel::MovementFSMLeaseComponent& lease,
		float clock
	)
	{
		if (req.severity < lease.severity) return;

		lease.issuerAxis = req.issuerAxis;
		//lease.issuerEntity
		lease.mode = req.mode;
		lease.forcedState = req.forcedState;
		lease.severity = req.severity;
		lease.remainingDurationSec = req.durationSec;

		std::cout << "[MovementFSMInterference]: forced state Transition accepted at " << clock 
			<< " seconds. -> (release at " << clock + lease.remainingDurationSec << " )\n";

	}

	void MovementFSMInterferenceResolver::updateInterference(
		Engine::ECS::EntityMgr& ecs,
		Engine::ECS::Entity e,
		Game::Character::FSM::Movement::StateModel::MovementFSMLeaseComponent& lease, 
		float clock,
		float dt
	)
	{
		constexpr float eps = 1e-6;

		if (!lease.isActive(eps))
		{
			// std::cout << "[MovementFSMInterferenceResolver] released at " << clock << "\n";
			lease.reset();// 残骸処理
			return;
		}

		lease.tick(dt);

		// switchは現在未使用：それほど大規模なの状態干渉動作を想定していない
		//switch (lease.mode)
		//{
		//case InterferenceMode::ForceTransition:

		//case InterferenceMode::BlockInput:
		//}
		
		if (!lease.hasTimeLeft(eps))
		{
			// std::cout << "[MovementFSMInterferenceResolver] released at " << clock << "\n";
			lease.reset();
		}

	}
}
