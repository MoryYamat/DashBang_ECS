#include "LifetimeSystem.hpp"

#include "Engine/ECS/EntityUtils/EntityUtils.h"

#include "Game/ECS/Component/LifetimeComponent.hpp"




void Game::ECS::System::UpdateLifetimeSystem(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	for (Engine::ECS::Entity e : ecs.view<Game::ECS::Component::LifetimeComponent>())
	{
		auto& lifetime = ecs.get<Game::ECS::Component::LifetimeComponent>(e);
		lifetime.elapsedTime += deltaTime;
		if (lifetime.elapsedTime >= lifetime.totalLifetime)
		{
			Engine::ECS::EntityUtils::MarkForPendingDestroy(ecs, e);
		}
	}
}