#include "AnimationArbiterSystem.hpp"

#include "Game/Character/Private/Animation/Arbiter/FinalAnimationDecisionComponent.hpp"

#include "Game/Character/Private/Animation/Resolve/Movement/MovementAnimDecisionComponent.hpp"
#include "Game/Character/Private/Animation/Resolve/Skill/SkillAnimDecisionComponent.hpp"
#include "Game/Character/Private/Animation/Resolve/CC/CCAnimDecisionComponent.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

namespace Game::Character::Animation::Arbiter
{
	namespace Ops = Engine::ECS::Ops;

	namespace Arbiter = Game::Character::Animation::Arbiter;
	namespace MvAnim = Game::Character::Animation::Resolve::Movement;
	namespace SkAnim = Game::Character::Animation::Resolve::Skill;
	namespace CCAnim = Game::Character::Animation::Resolve::CC;

	void AnimationArbiterSystem::Update(Engine::ECS::EntityMgr& ecs)
	{
		for (auto e : ecs.view<
			Arbiter::FinalAnimationDecisionComponent,
			MvAnim::MovementAnimDecisionComponent,
			SkAnim::SkillAnimDecisionComponent,
			CCAnim::CCAnimDecisionComponent
		>())
		{
			auto& out = Ops::Get<Arbiter::FinalAnimationDecisionComponent>(ecs, e);
			const auto& mv = Ops::Get<MvAnim::MovementAnimDecisionComponent>(ecs, e);
			const auto& sk = Ops::Get<SkAnim::SkillAnimDecisionComponent>(ecs, e);
			const auto& cc = Ops::Get<CCAnim::CCAnimDecisionComponent>(ecs, e);

			// TODO: 条件のハードコードをやめ、優先度による解決、データ駆動で解決できるようにする必要がある
			// FIXME:条件のハードコードをやめ、優先度による解決、データ駆動で解決できるようにする必要がある
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
			if (cc.valid)
			{
				out.valid = true;
				out.clipKey = cc.clipKey;
				out.loop = cc.loop;
				out.playRate = cc.playRate;
				// std::cout << "here\n";
			}

			// 将来的に CC / Skill の優先度ロジックを追記したりする
		}
	}
}