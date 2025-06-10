#include "CollisionSyncSystem.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"

void Game::Collision::System::CollisionSyncSystem(eNsECS::EntityMgr& ecs)
{
	for (eNsECS::Entity e : ecs.view<
		eNsLogic2DComp::Logic2DTransformComponent,
		eNsLogic2DComp::CollisionComponent>())
	{
		auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(e);
		auto& collision = ecs.get<eNsLogic2DComp::CollisionComponent>(e);

		//if (collision.collider.type == eNsLogic2DComp::ColliderType::Circle2D)
		//{
		//	collision.collider.circle2D.center = logic.positionXZ;
		//}
	}
}