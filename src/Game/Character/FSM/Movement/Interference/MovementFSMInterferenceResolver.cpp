#include "MovementFSMInterferenceResolver.hpp"

#include "Engine/Time/WorldClock.hpp"

#include "Game/Character/FSM/Interference/Core/Utils/ResolverUtil.hpp"

#include "Game/Character/FSM/Movement/MovementStateTags.hpp"

namespace Game::Character::FSM::Movement::Interference
{
	using namespace Engine::Time;

	using namespace Engine::ECS;
	using namespace Game::Character::FSM::Interference::Core::Data;
	using namespace Game::Character::FSM::Movement::StateModel;
	using namespace Game::Character::FSM::Movement;

	void MovementFSMInterferenceResolver::Update(EntityMgr& ecs, float deltaTime)
	{
		for (auto e : ecs.view<FSMInterferenceRequestComponent, MovementStateComponent, MovementFSMLeaseComponent>())
		{
			auto& request = ecs.get<FSMInterferenceRequestComponent>(e);
			auto& state = ecs.get<MovementStateComponent>(e);
			auto& lease = ecs.get<MovementFSMLeaseComponent>(e);


			if (auto selected = computeHighestPriorityRequest(ecs, e, request, state, lease))
			{
				acceptInterference(ecs, e, *selected, lease);
			}

			updateInterference(ecs, e, lease, deltaTime);// 状態は変えない (状態更新はFSMResovlerで)


		}
	}



	const FSMInterferenceRequest* MovementFSMInterferenceResolver::computeHighestPriorityRequest(
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
			// 有効なrequestか確認(durationSec>0 && targetAxis == AxisTag::MovementAxis)
			if (!shouldApply(req, targetAxis)) continue;
			// forcedStateがなければ無視
			if (!req.forcedState.has_value()) continue;
			// 現在のlease.severityより小さいなら無視
			if (req.severity < lease.severity) continue;

			// 
			if (!selected || req.severity > selected->severity)
				selected = &req;
		}

		if (!selected) return;
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
		Game::Character::FSM::Movement::StateModel::MovementFSMLeaseComponent& lease
	)
	{
		lease.issuerAxis = req.issuerAxis;
		//lease.issuerEntity
		lease.mode = req.mode;
		lease.forcedState = req.forcedState;
		lease.severity = req.severity;
		lease.remainingDurationSec = req.durationSec;
	}

	void MovementFSMInterferenceResolver::updateInterference(
		Engine::ECS::EntityMgr& ecs,
		Engine::ECS::Entity e,
		Game::Character::FSM::Movement::StateModel::MovementFSMLeaseComponent& lease,
		float dt
	)
	{
		constexpr float eps = 1e-6;

		if (!lease.isActive(eps))
		{
			lease.reset();// 残骸処理
			return;
		}

		lease.tick(dt);

		// switchは現在未使用：それほど大規模なの状態干渉動作を想定していない
		switch (lease.mode)
		{
		case InterferenceMode::ForceTransition:

		case InterferenceMode::BlockInput:
		}
		
		if (!lease.hasTimeLeft(eps))
		{
			lease.reset();
		}

	}
}
