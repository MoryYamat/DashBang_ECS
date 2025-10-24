#include "ApplyFinalAnimationDecisionSystem.hpp"

#include "Engine/ECS/Component/Graphics/MeshComponent.h"
#include "Engine/ECS/Component/Graphics/AnimatorComponent.hpp"

#include "Game/Character/Private/Animation/Arbiter/FinalAnimationDecisionComponent.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

namespace Game::Character::Animation::Apply
{
	namespace ECS = Engine::ECS;
	namespace Ops = Engine::ECS::Ops;
	
	namespace Grf = Engine::ECS::Component::Graphics;

	namespace Arbiter = Game::Character::Animation::Arbiter;

	void ApplyFinalAnimationDecisionSystem::Update(Engine::ECS::EntityMgr& ecs)
	{
		for (auto e : ecs.view<
			Grf::AnimatorComponent,
			Grf::MeshComponent,
			Arbiter::FinalAnimationDecisionComponent
		>())
		{
			auto& anim = Ops::Get<Grf::AnimatorComponent>(ecs, e);
			const auto& mesh = Ops::Get<Grf::MeshComponent>(ecs, e);
			const auto& dec = Ops::Get<Arbiter::FinalAnimationDecisionComponent>(ecs, e);

			if (!dec.valid) continue;

			// 同一クリップなら何もしない (無駄リセット防止)
			bool same = false;
			if (anim.clipIndex >= 0 && anim.clipIndex < (int)mesh.modelData.clips.size())
			{
				same = (mesh.modelData.clips[(size_t)anim.clipIndex].name == dec.clipKey);
			}
			if (!same)
			{
				if (!Grf::SetClipByName(mesh.modelData, anim, dec.clipKey, dec.loop))
				{
					// フォールバック (ログ推奨)
					Grf::SetClipByName(mesh.modelData, anim, "idle_default", true);
				}
			}

			anim.speed = dec.playRate;
		}
	}


}