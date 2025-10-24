#include "SkillAnimResolverSystem.hpp"

#include "Game/Character/Private/Animation/Query/Skill/SkillAnimationQueryComponent.hpp"
#include "Game/Character/Private/Animation/Resolve/Skill/SkillAnimDecisionComponent.hpp"
#include "Game/Character/Private/Animation/Profile/Skill/SkillAnimationProfile.hpp"


#include "Engine/ECS/Ops/CoreOps.hpp"

namespace Game::Character::Animation::Resolve::Skill
{
	namespace Ops = Engine::ECS::Ops;

	namespace sQuery = Game::Character::Animation::Query;

	namespace sProf = Game::Character::Animation::Profile;


	void SkillAnimResolverSystem::Update(Engine::ECS::EntityMgr& ecs)
	{
		for (auto e : ecs.view
			<sQuery::SkillAnimQueryComponent,
			SkillAnimDecisionComponent>())
		{
			auto& q = Ops::Get<sQuery::SkillAnimQueryComponent>(ecs, e);
			auto& dec = Ops::Get<SkillAnimDecisionComponent>(ecs, e);


			if (!q.valid)
			{
				dec.valid = false;
				continue;
			}

			if (!q.isSkillActive)
			{
				q.valid = false;
				dec.valid = false;
				continue;
			}
			const auto& adb = Ops::GetRes<sProf::SkillAnimationProfileDatabase>(ecs);// anim
			// const auto& animClip = adb.ResolveOrFallback("Paladin", q.skillId);
			//dec.clipKey = animClip;
			//dec.loop = true;
			//dec.playRate = 1.0f;
			//dec.valid = true;

			// TODO:
			const auto& meta = adb.ResolveOrFallback("Paladin", q.skillId);// ハードコード解消必要 //tagCompもしくはidから解決必要
			dec.clipKey = meta.clip;
			dec.playRate = meta.speed;
			dec.loop = true;
			dec.valid = true;
		}
	}
}