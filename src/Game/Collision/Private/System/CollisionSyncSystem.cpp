#include "CollisionSyncSystem.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"

// おそらく 未使用，削除予定
void Game::Collision::System::CollisionSyncSystem(Engine::ECS::EntityMgr& ecs)
{
	for (Engine::ECS::Entity e : ecs.view<
		Engine::ECS::Component::Logic2D::Logic2DTransformComponent,
		Engine::ECS::Component::Logic2D::CollisionComponent>())
	{
		auto& logic = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e);
		auto& collision = ecs.get<Engine::ECS::Component::Logic2D::CollisionComponent>(e);

		//if (collision.collider.type == Engine::ECS::Component::Logic2D::ColliderType::Circle2D)
		//{
		//	collision.collider.circle2D.center = logic.positionXZ;
		//}
	}
}