#include "Attack2DAreaLifetimeSystem.hpp"

#include "Game/Combat/Private/Skill/Component/Attack2DAreaComponent.h"
#include "Game/ECS/Private/Component/LifetimeComponent.hpp"

#include "Engine/ECS/EntityUtils/EntityUtils.h"


// 現在未使用(LifetimeComponentを持つエンティティはGame/ECS/Systemによる一括管理にしている)：削除予定
// 現在未使用(LifetimeComponentを持つエンティティはGame/ECS/Systemによる一括管理にしている)：削除予定
// 現在未使用(LifetimeComponentを持つエンティティはGame/ECS/Systemによる一括管理にしている)：削除予定
void Game::Combat::Skill::System::UpdateAttack2DAreaLifetimeSystem(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	for (Engine::ECS::Entity e : ecs.view<Game::Combat::Skill::Component::Attack2DAreaComponent, Game::ECS::Component::LifetimeComponent>())
	{
		auto& lifetime = ecs.get<Game::ECS::Component::LifetimeComponent>(e);
		lifetime.elapsedTime += deltaTime;

		if (lifetime.elapsedTime >= lifetime.totalLifetime)
		{
			Engine::ECS::EntityUtils::MarkForPendingDestroy(ecs, e);
		}
	}
}