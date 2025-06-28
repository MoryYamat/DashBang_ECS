#include "LifetimeSystem.hpp"

#include "Engine/ECS/EntityUtils/EntityUtils.h"

#include "Game/ECS/Component/LifetimeComponent.hpp"

#include "Common/GameNamespaceDecl.h"


void Game::ECS::System::UpdateLifetimeSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	for (eNsECS::Entity e : ecs.view<gNsECSComp::LifetimeComponent>())
	{
		auto& lifetime = ecs.get<gNsECSComp::LifetimeComponent>(e);
		lifetime.elapsedTime += deltaTime;
		if (lifetime.elapsedTime >= lifetime.totalLifetime)
		{
			eNsECS::EntityUtils::MarkForPendingDestroy(ecs, e);
		}
	}
}