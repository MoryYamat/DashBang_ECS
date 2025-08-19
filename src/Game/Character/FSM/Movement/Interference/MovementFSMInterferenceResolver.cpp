#include "MovementFSMInterferenceResolver.hpp"

#include "Game/Character/FSM/Interference/Core/Utils/ResolverUtil.hpp"

#include "Game/Character/FSM/Movement/MovementStateTags.hpp"

namespace Game::Character::FSM::Movement::Interference
{


	void MovementFSMInterferenceResolver::Update(EntityMgr& ecs, float deltaTime)
	{
		for (auto e : ecs.view<FSMInterferenceRequestComponent, MovementStateComponent, MovementFSMLeaseComponent>())
		{
			auto& request = ecs.get<FSMInterferenceRequestComponent>(e);
			auto& state = ecs.get<MovementStateComponent>(e);
			auto& lease = ecs.get<MovementFSMLeaseComponent>(e);

			resolveMovementFSMInterference(ecs, e, request, state, lease);
		}
	}



	void MovementFSMInterferenceResolver::resolveMovementFSMInterference(
		EntityMgr& ecs,
		Entity e,
		const FSMInterferenceRequestComponent& requestComp,
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
			if (!shouldApply(req, targetAxis)) continue;
			if (!req.forcedState.has_value()) continue;
			if (req.severity <= lease.severity) continue;

			if (!selected || req.severity > selected->severity)
				selected = &req;
		}

		if (!selected) return;

		// 強制遷移処理
		applyForcedTransition(lease, state, *selected);

		//lease.severity = selected->severity;
		//lease.remainingDurationSec = selected->durationSec;

		//state.current = *selected->forcedState;
	}
}
