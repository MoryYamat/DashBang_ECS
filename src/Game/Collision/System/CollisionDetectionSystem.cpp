#include "CollisionDetectionSystem.h"

#include "Engine/Time/WorldClock.hpp"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"

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

#include "Game/Combat/HitEvent/Database/HitEventDatabase.hpp"
#include "Game/Combat/HitEvent/API/GenerateHitEvent.hpp"

// Skill
#include "Game/Combat/Skill/Component/SkillExecutionContextComponent.hpp"
#include "Game/Combat/Skill/Component/SkillOwnerComponent.hpp"
#include "Game/Combat/Skill/Component/HitboxHitMemoComponent.hpp"
#include "Game/Combat/Skill/API/Public/tryGetHitboxHitMemoComponent.hpp"
// teamTag
#include "Game/ECS/Component/TeamComponent.h"

// HitEvent
#include "Game/Combat/HitEvent/Data/HitEvent.hpp"

#include "Engine/Debug/DebugUtils.h"

#include <glm/glm.hpp>

#include <iostream>

// 現役
void Game::Collision::System::UpdateCollisionResultBuffer(eNsECS::EntityMgr& ecs)
{
	using namespace Engine::Time;
	using namespace Game::Collision::Data;

	using namespace Game::Combat::Skill::Component;

	using namespace Game::ECS::Component;

	using namespace Game::Combat::HitEvent::Database;
	using namespace Game::Combat::HitEvent::Data;

	using namespace Game::Combat::Skill::API::External;

	// assert(ecs.hasResource<gNsCollData::CollisionResultBuffer>() && "CollisionResultBuffer not initialized");
	// if(!ecs.hasResource< gNsCollData::CollisionResultBuffer>()) return;
	const auto& clock = ecs.getResource<WorldClockData>();
	auto& hitDb = ecs.getResource<HitEventDatabase>();
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
			// 

			// カテゴリ両想い判定
			if (!gNsCollComp::shouldCollideWithCat(maskA, maskB))
				continue;

			// Relation 両想い 
			const auto rab = gNsCollComp::computeRelation(ecs, eA, eB);
			const auto rba = gNsCollComp::computeRelation(ecs, eB, eA);
			if (!gNsCollComp::shouldCollideWithRel(maskA, maskB, rab, rba)) continue;

			// 衝突判定済みかどうかの記録(memo)を確認
			const bool aIsSkill = isSkillEntity(ecs, eA);
			const bool bIsSkill = isSkillEntity(ecs, eB);

			HitboxHitMemoComponent* memoA = tryGetMemo(ecs, eA);
			HitboxHitMemoComponent* memoB = tryGetMemo(ecs, eB);

			if (aIsSkill && memoA && memoA->alreadyHit(eB)) continue;
			if (bIsSkill && memoB && memoB->alreadyHit(eA))continue;


			// 判定用一般化形状変換
			auto shapeA = gNsCollConvert::MakeGenericShape2D(eA, ecs);
			auto shapeB = gNsCollConvert::MakeGenericShape2D(eB, ecs);
			if (!gNsCollIntersect::Intersects(shapeA, shapeB)) continue;

			//std::visit([](auto&& s) {
			//	std::cout << "[shapeA] type: " << typeid(s).name() << std::endl;
			//	}, shapeA);

			//std::visit([](auto&& s) {
			//	std::cout << "[shapeB] type: " << typeid(s).name() << std::endl;
			//	}, shapeB);


			// バッファへ追加
				// 仮のContactInfo（後で精密な法線・深度が必要になれば拡張）
			gNsCollData::ContactInfo info{ //.contactNormal = glm::normalize(transB.positionXZ - transA.positionXZ), 
				.penetrationDepth = 0.0f };
			// std::cout << "[CollisionDetectionSystem]: collider detcted" << std::endl;
			// バッファ追加
			buffer.add(gNsCollData::CollisionResult{ eA, eB, info });


			// FIXME isSkillEntityの再利用
			// =================================================================
			// Skill & target について
			eNsECS::Entity skillEnt{}, targetEnt{};
			SkillOwnerComponent meta{};
			if (isSkillEntity(ecs, eA) && !isSkillEntity(ecs, eB))
			{
				skillEnt = eA; targetEnt = eB;
				meta = ecs.get<SkillOwnerComponent>(eA);
				// relによって解決済みの可能性あり．よって不要かも
				// if(meta.caster == targetEnt)
				// => skillIDに基づいて効果を適用するかどうかをキチンと判断しなければいけない
			}
			else if (isSkillEntity(ecs, eB) && !isSkillEntity(ecs, eA))
			{
				skillEnt = eB; targetEnt = eA;
				meta = ecs.get<SkillOwnerComponent>(eB);

			}
			else
			{
				continue;
			}

			// FIXME: API化
			// 以下に HitEvent生成処理を追加
			//HitEvent ev{};
			//ev.skill = skillEnt;
			//ev.target = targetEnt;
			//ev.skillId = meta.skillId;
			//ev.SpawnTime = clock.now;

			// memo更新
			if (aIsSkill && memoA)
			{
				memoA->markHit(eB);
			}
			if (bIsSkill && memoB)
			{
				memoB->markHit(eA);
			}

			Game::Combat::HitEvent::API::GenerateHitEvent(ecs, skillEnt, targetEnt, meta.skillId, clock.now, hitDb);

			//hitDb.push(std::move(ev));
		}
	}
}

namespace Game::Collision::System
{
	using namespace Game::Combat::Skill::Component;

	// TODO: 関数の場所整理
	bool isSkillEntity(eNsECS::EntityMgr& ecs, eNsECS::Entity entity)
	{
		return ecs.hasComponent<SkillOwnerComponent>(entity);
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



