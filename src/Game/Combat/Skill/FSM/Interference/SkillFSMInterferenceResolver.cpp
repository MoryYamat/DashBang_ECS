#include "SkillFSMInterferenceResolver.hpp"

#include "Engine/Time/WorldClock.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillStateComponent.hpp"

#include "Game/Character/FSM/Interference/Core/Utils/ResolverUtil.hpp"

namespace Game::Combat::Skill::FSM::Interference
{

	namespace StateModel = Game::Combat::Skill::FSM::StateModel;
	namespace IF = Game::Character::FSM::Interference::Core::Data;
	namespace ECS = Engine::ECS;

	namespace AxisTag = Game::Combat::Skill::FSM::AxisTag;

	void SkillFSMInterferenceResolver::Update(Engine::ECS::EntityMgr& ecs)
	{
		const auto& clock = Engine::Time::worldClock(ecs);

		for (auto e : ecs.view<
			StateModel::SkillStateComponent,
			StateModel::SkillFSMLeaseComponent,
			StateModel::SkillFSMInterferenceRequestComponent
		>())
		{
			auto& state = ecs.get<StateModel::SkillStateComponent>(e);
			auto& lease = ecs.get<StateModel::SkillFSMLeaseComponent>(e);
			auto& reqs = ecs.get<StateModel::SkillFSMInterferenceRequestComponent>(e);

			if (const auto selected = computeHighestPriorityRequest(ecs, e, reqs))
			{
				acceptInterference(ecs, e, *selected, lease, clock.now);
			}

				updateInterference(ecs, e, lease, clock.now, clock.dt);

			reqs.requests.clear();
		}
	}

	const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest*
		SkillFSMInterferenceResolver::computeHighestPriorityRequest(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity e,
			Game::Combat::Skill::FSM::StateModel::SkillFSMInterferenceRequestComponent& reqs
		)
	{
		namespace IFUtils =  Game::Character::FSM::Interference::Core::Util;


		const IF::FSMInterferenceRequest* selected = nullptr;

		const auto targetAxis = AxisTag::SKILLAXIS;

		for (const auto& req : reqs.requests)
		{
			if (IFUtils::shouldApply(req, targetAxis)) continue;
			if (!req.forcedState.has_value()) continue;

			if (!selected || req.severity > selected->severity) 
				selected = &req;
		}

		if (!selected) return nullptr;
		return selected;


	}

	void SkillFSMInterferenceResolver::acceptInterference(
		Engine::ECS::EntityMgr& ecs,
		Engine::ECS::Entity e,
		const Game::Character::FSM::Interference::Core::Data::FSMInterferenceRequest& req,
		Game::Combat::Skill::FSM::StateModel::SkillFSMLeaseComponent& lease,
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

		std::cout << "[SkillFSMInterferenceResolver]: forced state Transition accepted at " << clock
			<< " seconds. -> (release at " << clock + lease.remainingDurationSec << " )\n";
	}

	void SkillFSMInterferenceResolver::updateInterference(
		Engine::ECS::EntityMgr& ecs,
		Engine::ECS::Entity e,
		Game::Combat::Skill::FSM::StateModel::SkillFSMLeaseComponent& lease,
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