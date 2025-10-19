// スキルにより生成されたHitbox Entityの寿命をstateなどの依存を考慮してコントロールするシステム

#pragma once

#include "Engine/ECS/EntityManager.h"



namespace Game::Combat::Skill::System
{
	class HitboxLifetimeControlSystem
	{
	public:
		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}