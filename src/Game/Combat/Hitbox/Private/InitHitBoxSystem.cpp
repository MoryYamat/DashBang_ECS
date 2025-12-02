#include "Game/Combat/HitBox/Public/HitBoxApi.hpp"

#include "Game/Combat/HitBox/Public/HitBoxTypes.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Combat::HitBox
{
	void InitAllHitBoxSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		// Database 作成


		// 初期化



		// buffer 作成 / 初期化
		auto& buf = ctx.ww.CreateResource<HitBoxSpawnRequestBuffer>();
		buf.clear();


	}
}