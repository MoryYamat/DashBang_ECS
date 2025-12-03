#include "Game/Combat/HitBox/Public/HitBoxTypes.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"
#include "Engine/Component/Private/Common/LifetimeComponent.hpp"
#include "Engine/Time/Private/WorldClock.hpp"


#include "Engine/Log/Public/LogApi.hpp"


namespace Game::Combat::HitBox
{
	using namespace Engine::Component;
	void HitBoxSpawnSystem::Update(const float dt)
	{
		auto& catalog = ctx.ww.GetResource<HitBoxCatalog>();
		auto& reqBuf = ctx.ww.GetResource<HitBoxSpawnRequestBuffer>();

		if (reqBuf.reqs.empty())
			return;

		for (const auto& req : reqBuf.reqs)
		{
			if (!req.hitbox.valid())continue;

			const auto idx = req.hitbox.v;
			if (idx >= catalog.hitBoxes.size()) continue;
			const auto& hd = catalog.hitBoxes[idx];

			auto e = ctx.ww.Create();

			// Transform / Position
			// owner の 位置 + hd.offset から 初期位置決定
			const auto* ownerPos = ctx.rw.TryGet<Logic2DTransformComponent>(req.owner);
			if (!ownerPos) 
			{
				Engine::Log::Write(Engine::Log::Level::Info, "HitBoxSpawnSystem",
					"The OwnerEntity of the HitBox does not have a Logic2DComponent.");
				continue;
			}

			Logic2DTransformComponent lg{};
			lg.positionXZ = ownerPos->positionXZ + hd.offset;
			lg.front = ownerPos->front;
			lg.right = ownerPos->right;
			lg.rotation = ownerPos->rotation;
			lg.scale = ownerPos->scale;
			ctx.ww.Add<Logic2DTransformComponent>(e, lg);

			LifetimeComponent lifetime{};
			lifetime.remainingLifetime = req.lifetime;
			ctx.ww.Add<LifetimeComponent>(e, lifetime);

			// HitBoxComponent
			HitBoxComponent hb{};
			hb.def = req.hitbox;
			hb.owner = req.owner;
			hb.skill = req.skill;
			ctx.ww.Add<HitBoxComponent>(e, hb);
		}

		reqBuf.clear();
	}

	void UpdateAllHitBoxSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		const auto& clock = ctx.rw.GetResource<Engine::Time::WorldClockData>();

		HitBoxSpawnSystem hbSpawn{ ctx };
		hbSpawn.Update(clock.fixedDt);
	}
}