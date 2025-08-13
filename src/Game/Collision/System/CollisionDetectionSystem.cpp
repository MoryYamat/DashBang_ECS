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
	using namespace Game::Collision::Data;
	// assert(ecs.hasResource<gNsCollData::CollisionResultBuffer>() && "CollisionResultBuffer not initialized");
	auto& buffer = ecs.getResource<gNsCollData::CollisionResultBuffer>();
	// auto& bus = ecs.getResource<ContactBus>();
	// std::cout << "[CollisionDetectionSystem.cpp()]Collision Count: " << buffer.results.size() << std::endl;

	// バッファクリア
	buffer.clear();
	// bus.clear();

	//auto entities = ecs.view <
	//	eNsLogic2DComp::CollisionComponent,
	//	gNsCollComp::CollisionMaskComponent,
	//	eNsLogic2DComp::Logic2DTransformComponent>();

	// Must=>And , Any=>OR
	// using Must = std::tuple<gNsCollComp::CollisionMaskComponent, eNsLogic2DComp::CollisionComponent>;
	using Must = std::tuple<gNsCollComp::CollisionMaskComponent>;
	// CollisionComponentをMustにすると，Skillに対応できない問題
	// CollisionComponentをMustにすると，Skillに対応できない問題
	// CollisionComponentをMustにすると，Skillに対応できない問題
	
	using Any = std::tuple<eNsLogic2DComp::Logic2DTransformComponent, eNsLogic2DComp::Transform2DComponent>;

	auto entities = ecs.view(eNsECS::EntityMgr::FilterSpec<Must, Any>{});

	// スキル発生時にTransform2DComponentを検索できていない
	//for (auto e : entities)
	//{
	//	std::cout << "Entity: " << e.id << std::endl;
	//	std::cout << "  Has Mask: " << ecs.hasComponent<gNsCollComp::CollisionMaskComponent>(e) << std::endl;
	//	std::cout << "  Has Collider: " << ecs.hasComponent<eNsLogic2DComp::CollisionComponent>(e) << std::endl;
	//	std::cout << "  Has Logic2DTransform: " << ecs.hasComponent<eNsLogic2DComp::Logic2DTransformComponent>(e) << std::endl;
	//	std::cout << "  Has Transform2D: " << ecs.hasComponent<eNsLogic2DComp::Transform2DComponent>(e) << std::endl;
	//}

	for (size_t i = 0; i < entities.size(); ++i)
	{
		for (size_t j = i + 1; j < entities.size(); ++j)
		{
			eNsECS::Entity eA = entities[i];
			eNsECS::Entity eB = entities[j];


			auto& maskA = ecs.get<gNsCollComp::CollisionMaskComponent>(eA);
			auto& maskB = ecs.get<gNsCollComp::CollisionMaskComponent>(eB);

			// std::cout << "[CollisionDetectionSystem.cpp()]: before mask judge\n";
			if (!gNsCollUtil::shouldCollide(maskA, maskB))
				continue;


			auto shapeA = gNsCollConvert::MakeGenericShape2D(eA, ecs);
			auto shapeB = gNsCollConvert::MakeGenericShape2D(eB, ecs);
			if(!gNsCollIntersect::Intersects(shapeA, shapeB)) continue;
			
			//std::visit([](auto&& s) {
			//	std::cout << "[shapeA] type: " << typeid(s).name() << std::endl;
			//	}, shapeA);

			//std::visit([](auto&& s) {
			//	std::cout << "[shapeB] type: " << typeid(s).name() << std::endl;
			//	}, shapeB);

			if (gNsCollIntersect::Intersects(shapeA, shapeB))
			{
				// 仮のContactInfo（後で精密な法線・深度が必要になれば拡張）
				gNsCollData::ContactInfo info{ //.contactNormal = glm::normalize(transB.positionXZ - transA.positionXZ), 
					.penetrationDepth = 0.0f };

				// std::cout << "[CollisionDetectionSystem]: collider detcted" << std::endl;
				// バッファ追加
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



