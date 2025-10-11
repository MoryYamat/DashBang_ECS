#include "AnimationArbiterSystem.hpp"

#include "Game/Character/Animation/Arbiter/FinalAnimationDecisionComponent.hpp"

#include "Game/Character/Animation/Resolve/Movement/MovementAnimDecisionComponent.hpp"

#include "Game/Character/Animation/Resolve/Skill/SkillAnimDecisionComponent.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

namespace Game::Character::Animation::Arbiter
{
	namespace Ops = Engine::ECS::Ops;

	namespace Arbiter = Game::Character::Animation::Arbiter;
	namespace MvAnim = Game::Character::Animation::Resolve::Movement;
	namespace SkAnim = Game::Character::Animation::Resolve::Skill;

	void AnimationArbiterSystem::Update(Engine::ECS::EntityMgr& ecs)
	{
		for (auto e : ecs.view<
			Arbiter::FinalAnimationDecisionComponent,
			MvAnim::MovementAnimDecisionComponent,
			SkAnim::SkillAnimDecisionComponent
		>())
		{
			auto& out = Ops::Get<Arbiter::FinalAnimationDecisionComponent>(ecs, e);
			const auto& mv = Ops::Get<MvAnim::MovementAnimDecisionComponent>(ecs, e);
			const auto& sk = Ops::Get<SkAnim::SkillAnimDecisionComponent>(ecs, e);

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
			if (sk.valid)
			{
				out.valid = true;
				out.clipKey = sk.clipKey;
				out.loop = sk.loop;
				out.playRate = sk.playRate;
			}
			//else
			//{
			//	out.valid = false;
			//}

			// 将来的に CC / Skill の優先度ロジックを追記したりする
		}
	}
}