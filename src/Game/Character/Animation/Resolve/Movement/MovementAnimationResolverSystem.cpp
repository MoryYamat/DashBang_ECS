#include "MovementAnimationResolverSystem.hpp"


#include "Game/Character/Animation/Profile/AnimationProfileComponent.hpp"

#include "Game/Character/Animation/Profile/Movement/MovementAnimationProfile.hpp"


#include "Game/Character/Animation/Query/AnimationQueryComponent.hpp"
#include "Game/Character/Animation/Resolve/Movement/MovementAnimDecisionComponent.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <cassert>

namespace Game::Character::Animation::Resolve::Movement
{
	namespace ECS = Engine::ECS;
	namespace Ops = Engine::ECS::Ops;

	namespace Query = Game::Character::Animation::Query;
	namespace AnimResolve = Game::Character::Animation::Resolve::Movement;
	namespace Prof = Game::Character::Animation::Profile;

	void MovementAnimationResolverSystem::Update(ECS::EntityMgr& ecs)
	{
		auto& db = Ops::GetOrCreateRes<Prof::MovementAnimationProfileDatabase>(ecs);

		for (auto e : ecs.view<
			Query::AnimationQueryComponent,
			AnimResolve::MovementAnimDecisionComponent,
			Prof::AnimationProfileComponent
		>())
		{
			const auto& q = Ops::Get<Query::AnimationQueryComponent>(ecs, e);
			const auto& prof = Ops::Get<Prof::AnimationProfileComponent>(ecs, e);
			auto& dec = Ops::Get<AnimResolve::MovementAnimDecisionComponent>(ecs, e);



			if (!q.valid)
			{
				dec.valid = false;
				continue;
			}

			dec.type = q.isMoving ? AnimResolve::MoveAnimType::RunFwd : AnimResolve::MoveAnimType::Idle;

			const auto* mp = db.FindMovement(prof.profileId);
			assert(mp);
		

			// TODO: switchは著しく柔軟性を損なうため、ほかの方法によって解決できるようにしたい
			switch (dec.type)
			{
			case AnimResolve::MoveAnimType::Idle: dec.clipKey = mp->idle.empty() ? "idle_default" : mp->idle; break;
			case AnimResolve::MoveAnimType::RunFwd: dec.clipKey = mp->runFwd.empty() ? "run_default" : mp->runFwd; break;
			}

			dec.loop = true;
			dec.playRate = 1.0f;
			dec.valid = true;
		}
	}
}