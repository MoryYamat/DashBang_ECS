#include "CollisionDetectionSystem.h"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Tags/PlayerCharacterTag.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Game/Collision/Component/CollisionMaskComponent.h"

#include "Game/Collision/Intersect/IntersectsGenericShape2D.h"

#include "Game/Collision/Convert/GenericShape2DConverter.h"
#include "Game/Collision/Utils/CollisionFilter.h"

#include "Game/Collision/Extractor/PlayerTileCollisionExtractor.h"

#include "Game/ECS/Tags/CharacterAttribTags.h"

// 
#include "Game/Collision/Data/CollisionContextData.h"
#include "Game/Collision/Data/CollisionResultStorage.h"

#include "Engine/Debug/DebugUtils.h"

#include <glm/glm.hpp>

#include <iostream>

void Game::Collision::System::UpdateCollisionResultBuffer(eNsECS::EntityMgr& ecs)
{
	auto& buffer = ecs.getResource<gNsCollData::CollisionResultBuffer>();
	buffer.clear();

	auto entities = ecs.view <
		eNsLogic2DComp::CollisionComponent,
		gNsCollComp::CollisionMaskComponent,
		eNsLogic2DComp::Logic2DTransformComponent>();

	for (size_t i = 0; i < entities.size(); ++i)
	{
		for (size_t j = i + 1; j < entities.size(); ++j)
		{
			eNsECS::Entity eA = entities[i];
			eNsECS::Entity eB = entities[j];

			auto& maskA = ecs.get<gNsCollComp::CollisionMaskComponent>(eA);
			auto& maskB = ecs.get<gNsCollComp::CollisionMaskComponent>(eB);

			if (!gNsCollUtil::shouldCollide(maskA, maskB))
				continue;

			auto& colA = ecs.get<eNsLogic2DComp::CollisionComponent>(eA);
			auto& colB = ecs.get<eNsLogic2DComp::CollisionComponent>(eB);



			auto& transA = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(eA);
			auto& transB = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(eB);

			auto shapeA = gNsCollConvert::MakeGenericShape2D(eA, ecs);
			auto shapeB = gNsCollConvert::MakeGenericShape2D(eB, ecs);

			if (gNsCollIntersect::Intersects(shapeA, shapeB))
			{
				// 仮のContactInfo（後で精密な法線・深度が必要になれば拡張）
				gNsCollData::ContactInfo info{ .contactNormal = glm::normalize(transB.positionXZ - transA.positionXZ), .penetrationDepth = 0.0f };
				buffer.add(gNsCollData::CollisionResult{ eA, eB, info });
			}
		}
	}
}

void Game::Collision::System::UpdateCollisionResultStorage(eNsECS::EntityMgr& ecs, gNsCollData::CollisionResultStorage& collisionResultStorage)
{
	//Game::Collision::Data::PlayerCollisionContext playerCollisionCtx;

	//for (eNsECS::Entity e : ecs.view<gNsTags::PlayerCharacterTag, eNsLogic2DComp::Logic2DTransformComponent, eNsLogic2DComp::CollisionComponent>())
	//{
	//	const auto& collisionComp = ecs.get<eNsLogic2DComp::CollisionComponent>(e);
	//	const auto& logic2DComp = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(e);

	//	// 更新 (名残，形状情報／変換情報の明確な責務分離後未使用)
	//	playerCollisionCtx.center = logic2DComp.positionXZ;
	//	// playerCollisionCtx.radius = collisionComp.collider.circle2D.radius;
	////	playerCollisionCtx.playerEntity = e;

	//	break;
	//}

	//eNsLogic2DComp::TileMapComponent tileMapComp;
	//for (eNsECS::Entity e : ecs.view<eNsLogic2DComp::TileMapComponent>())
	//{
	//	tileMapComp = ecs.get<eNsLogic2DComp::TileMapComponent>(e);

	//	std::vector<glm::ivec2> hitTileIndices = Game::Collision::Extract::ExtractPlayerTileCollisions(playerCollisionCtx, tileMapComp);

	//	collisionResultStorage.AddTileCollision(hitTileIndices);
	//}
}




void Game::Collision::System::CollisionDetectionSystem(eNsECS::EntityMgr& ecs, gNsCollData::CollisionResultStorage& collisionResultStorage)
{

}



