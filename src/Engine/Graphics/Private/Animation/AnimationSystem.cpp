#include "AnimationSystem.hpp"

//
#include "Engine/Time/Private/WorldClock.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Component/Private/Graphics/MeshComponent.hpp"
//

#include "Engine/ECS/Component/Graphics/MeshComponent.h"
#include "Engine/ECS/Component/Graphics/AnimatorComponent.hpp"

#include "Engine/Graphics/Private/Animation/AnimationEval.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <algorithm>

namespace Engine::Graphics::Animation::System
{
	namespace Opt = Engine::ECS::Ops;
	namespace Time = Engine::Time;

	namespace Graphic = Engine::ECS::Component::Graphics;

	namespace Anim = Engine::Graphics::Animation;
	
	void AnimationSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		ctx.ww.ForEachAlive([&](Engine::ECS::Core::Entity e) {
			auto* anm = ctx.ww.TryGet<Graphic::AnimatorComponent>(e);
			auto* ms = ctx.ww.TryGet<Engine::Component::MeshComponent>(e);

			if (!anm || !ms) return;

			const auto& model = ms->modelData;

			auto& anim = *anm;
			auto& mesh = *ms;

			// 基本ガード
			if (model.skeleton.bones.empty() || model.clips.empty()) return;

			if (anim.clipIndex < 0 || anim.clipIndex >= (int)model.clips.size()) return;



			const auto& clip = model.clips[(size_t)anim.clipIndex];

			const auto& clock = ctx.rw.GetResource<Engine::Time::WorldClockData>();
			// std::cout << "[animationSystem] deltaTime: " << clock.dt << "\n";

			// 時刻更新
			anim.time += clock.dt * anim.speed;

			if (anim.loop && clip.duration > 0.f)
			{
				anim.time = std::fmod(anim.time, clip.duration);
				if (anim.time < 0.f) anim.time += clip.duration;
			}
			else
			{
				anim.time = std::clamp(anim.time, 0.0f, clip.duration);
			}

			// クリップ評価 -> パレット
			Anim::SampleClip(model, clip, anim.time, anim.localTRS, false);
			//const auto& skel = model.skeleton;
			//anim.localTRS.resize(skel.bones.size());
			//for (size_t i = 0; i < skel.bones.size(); ++i) {
			//	anim.localTRS[i].t = skel.bones[i].defT;  // bind時ローカル平行移動
			//	anim.localTRS[i].r = skel.bones[i].defR;  // bind時ローカル回転
			//	anim.localTRS[i].s = skel.bones[i].defS;  // bind時ローカルスケール
			//}
			Anim::BuildPalette(model, anim.localTRS, anim.palette);
		});
	}


	// 削除予定
	void AnimationSystem(Engine::ECS::EntityMgr& ecs)
	{
		//Opt::View<Graphic::AnimatorComponent, Graphic::MeshComponent>(ecs)

		for (auto e : ecs.view
			<Graphic::AnimatorComponent, Graphic::MeshComponent>())
		{
			auto& anim = ecs.get<Graphic::AnimatorComponent>(e);
			auto& mesh = ecs.get<Graphic::MeshComponent>(e);

			const auto& model = mesh.modelData;



			// 基本ガード
			if (model.skeleton.bones.empty() || model.clips.empty()) continue;

			if (anim.clipIndex < 0 || anim.clipIndex >= (int)model.clips.size()) continue;



			const auto& clip = model.clips[(size_t)anim.clipIndex];

			const auto& clock = Time::worldClock(ecs);
			// std::cout << "[animationSystem] deltaTime: " << clock.dt << "\n";

			// 時刻更新
			anim.time += clock.dt * anim.speed;

			if (anim.loop && clip.duration > 0.f)
			{
				anim.time = std::fmod(anim.time, clip.duration);
				if (anim.time < 0.f) anim.time += clip.duration;
			}
			else
			{
				anim.time = std::clamp(anim.time, 0.0f, clip.duration);
			}

			// クリップ評価 -> パレット
			Anim::SampleClip(model, clip, anim.time, anim.localTRS, false);
			//const auto& skel = model.skeleton;
			//anim.localTRS.resize(skel.bones.size());
			//for (size_t i = 0; i < skel.bones.size(); ++i) {
			//	anim.localTRS[i].t = skel.bones[i].defT;  // bind時ローカル平行移動
			//	anim.localTRS[i].r = skel.bones[i].defR;  // bind時ローカル回転
			//	anim.localTRS[i].s = skel.bones[i].defS;  // bind時ローカルスケール
			//}
			Anim::BuildPalette(model, anim.localTRS, anim.palette);
		}
	}
}