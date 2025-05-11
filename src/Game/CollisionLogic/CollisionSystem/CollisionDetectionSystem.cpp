#include "CollisionDetectionSystem.h"

#include "Core/ECS/Component/Collision/CollisionComponent.h"
#include "Core/ECS/Component/PlayerControllerComponent.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"

#include "Core/ECS/Component/TileMapComponent.h"

#include "Game/CollisionLogic/CollisionExtractor/PlayerTileCollisionExtractor.h"

#include "Game/CollisionLogic/Data/CollisionContextData.h"

#include "Debug/DebugUtils.h"

#include <glm/glm.hpp>

#include <iostream>

void GameUtils::CollisionLogic::DetectionSystem::UpdateCollisionResultStorage(ECS& ecs, CollisionResultStorage& collisionResultStorage)
{
	GameUtils::CollisionLogic::Data::PlayerCollisionContext playerCollisionCtx;

	for (Entity e : ecs.view<PlayerControllerComponent, Logic2DTransformComponent, CollisionComponent>())
	{
		const auto& collisionComp = ecs.get<CollisionComponent>(e);
		const auto& logic2DComp = ecs.get<Logic2DTransformComponent>(e);

		playerCollisionCtx.center = collisionComp.collider.circle2D.center;
		playerCollisionCtx.radius = collisionComp.collider.circle2D.radius;
		// playerCollisionContext.playerEntity = e;

		break;
	}

	TileMapComponent tileMapComp;
	for (Entity e : ecs.view<TileMapComponent>())
	{
		tileMapComp = ecs.get<TileMapComponent>(e);

		std::vector<glm::ivec2> hitTileIndices = GameUtils::CollisionLogic::Extractor::ExtractPlayerTileCollisions(playerCollisionCtx, tileMapComp);

		collisionResultStorage.AddTileCollision(hitTileIndices);
	}
}


void GameUtils::CollisionLogic::DetectionSystem::CollisionDetectionSystem(ECS& ecs, CollisionResultStorage& collisionResultStorage)
{

}



