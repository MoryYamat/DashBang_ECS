#include "Attack2DAreaLifetimeSystem.hpp"

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"
#include "Game/ECS/Component/LifetimeComponent.hpp"

#include "Engine/ECS/EntityUtils/EntityUtils.h"

#include "Common/GameNamespaceDecl.h"

void Game::Combat::Skill::System::UpdateAttack2DAreaLifetimeSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	for (eNsECS::Entity e : ecs.view<gNsSkillComp::Attack2DAreaComponent, gNsECSComp::LifetimeComponent>())
	{
		auto& lifetime = ecs.get<gNsECSComp::LifetimeComponent>(e);
		lifetime.elapsedTime += deltaTime;

		if (lifetime.elapsedTime >= lifetime.totalLifetime)
		{
			eNsECS::EntityUtils::MarkForPendingDestroy(ecs, e);
		}
	}
}