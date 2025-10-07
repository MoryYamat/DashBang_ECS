#include "AnimationArbiterSystem.hpp"

#include "Game/Character/Animation/Arbiter/FinalAnimationDecisionComponent.hpp"

#include "Game/Character/Animation/Resolve/Movement/MovementAnimDecisionComponent.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

namespace Game::Character::Animation::Arbiter
{
	namespace Ops = Engine::ECS::Ops;

	namespace Arbiter = Game::Character::Animation::Arbiter;
	namespace MvAnim = Game::Character::Animation::Resolve::Movement;

	void AnimationArbiterSystem::Update(Engine::ECS::EntityMgr& ecs)
	{
		for (auto e : ecs.view<
			Arbiter::FinalAnimationDecisionComponent,
			MvAnim::MovementAnimDecisionComponent
		>())
		{
			auto& out = Ops::Get<Arbiter::FinalAnimationDecisionComponent>(ecs, e);
			const auto& mv = Ops::Get<MvAnim::MovementAnimDecisionComponent>(ecs, e);

			if (mv.valid)
			{
				out.valid = true;
				out.clipKey = mv.clipKey;
				out.loop = mv.loop;
				out.playRate = mv.playRate;
			}
			else
			{
				out.valid = false;
			}

			// 将来的に CC / Skill の優先度ロジックを追記したりする
		}
	}
}