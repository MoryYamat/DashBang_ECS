// スキルにより生成されたHitbox Entityの寿命をstateなどの依存を考慮してコントロールするシステム

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Combat::Skill::System
{
	class HitboxLifetimeControlSystem
	{
	public:
		static void Update(eNsECS::EntityMgr& ecs, float deltaTime);
	};
}