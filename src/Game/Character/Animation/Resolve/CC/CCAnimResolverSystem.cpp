#include "CCAnimResolverSystem.hpp"

#include "CCAnimDecisionComponent.hpp"

#include "Game/Character/Animation/Query/CC/CCAnimationQueryComponent.hpp"
#include "Game/Character/Animation/Profile/CC/CCAnimationProfile.hpp"


#include "Engine/ECS/Ops/CoreOps.hpp"

namespace Game::Character::Animation::Resolve::CC
{
	namespace Ops = Engine::ECS::Ops;

	using CCQuery = Game::Character::Animation::Query::CC::CCAnimationQueryComponent;
	using CCProf = Game::Character::Animation::Profile::CC::CCAnimationProfileDatabase;

	void CCAnimResolverSystem::Update(Engine::ECS::EntityMgr& ecs)
	{
		for (auto e : ecs.view<
			CCQuery,
			CCAnimDecisionComponent>())
		{
			auto& q = Ops::Get<CCQuery>(ecs, e);
			auto& dec = Ops::Get<CCAnimDecisionComponent>(ecs, e);

			if (!q.valid)
			{
				dec.valid = false;
				continue;
			}
			if (!q.isCCActive)
			{
				q.valid = false;
				dec.valid = false;
				continue;
			}

			// TODO: 名前解決ハードコード改善必要
			const auto& ccprof = Ops::GetRes<CCProf>(ecs);
			const auto& meta = ccprof.ResolveOrFallback("default", q.current);
			// std::cout << "current " << q.current.name();
			dec.clipKey = meta.clipKey;
			dec.loop = meta.loop;
			dec.playRate = meta.speed;
			dec.valid = true;
		}
	}
}