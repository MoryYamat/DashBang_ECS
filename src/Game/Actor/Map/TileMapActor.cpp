#include "TileMapActor.h"

#include "Engine/ECS/Entity.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"

#include "Engine/ECS/Component/Tags/TileMapTag.h"

#include "Game/Collision/Component/CollisionMaskComponent.h"

#include "Game/Init/InitModel/InitLogicTransformFromModel.h"
#include "Game/Init/InitTileMap/InitTileMap.h"

#include "Engine/Debug/DebugUtils.h"

#include <cstdint>

eNsECS::Entity Game::Actor::Map::TileMapActor::Create(
	eNsECS::EntityMgr& ecs,
	const eNsCommonComp::TransformComponent& transform,
	const eNsGfxModel::ModelData& modelData,
	float tileSize
)
{
	eNsECS::Entity entity = ecs.createEntity();

	// コンポーネント初期化
	eNsLogic2DComp::Logic2DTransformComponent logic2DComp = 
		gNsInit::Logic2D::InitLogic2DTransformFromModel(transform, modelData);
	ecs.addComponent(entity, logic2DComp);

	eNsLogic2DComp::TileMapComponent tileMapComp;
	tileMapComp.tileSize = tileSize;
	tileMapComp = gNsInit::Logic2D::InitTileMapFromBounds(transform, modelData, logic2DComp, tileSize);
	gNsInit::Logic2D::InitTileMapTiles(tileMapComp);

	ecs.addComponent(entity, tileMapComp);

	// Collision Component
	eNsLogic2DComp::CollisionComponent collisionComp;
	// calc world size on the xz plane
	collisionComp.collider.type = eNsLogic2DComp::ColliderType::Obb2D;
	collisionComp.collider.obb2D.center = logic2DComp.positionXZ;
	glm::vec2 worldSize = gNsInit::Logic2D::GetModelXZSizeWithScale(transform, modelData);
	collisionComp.collider.obb2D.halfExtents = worldSize * 0.5f;

	// calc world center on the xz plane
	glm::vec3 localCenter = modelData.GetCenter();
	glm::vec3 worldCenter3D = transform.toMatrix() * glm::vec4(localCenter, 1.0f);
	glm::vec2 worldCenterXZ = glm::vec2(worldCenter3D.x, worldCenter3D.z);
	collisionComp.collider.obb2D.center = worldCenterXZ;

	float rotRad = logic2DComp.rotation;// 描画基準と論理基準の整合性を考える
	// Front = Z axis basis
	glm::vec2 axisZ = glm::normalize(eNsLogic2DMath::CalcForwardFromYaw((rotRad)));
	glm::vec2 axisX = eNsLogic2DMath::CalcRightFromYaw(rotRad);
	collisionComp.collider.obb2D.axisX = axisX;
	collisionComp.collider.obb2D.axisZ = axisZ;
	ecs.addComponent(entity, collisionComp);

	ecs.addComponent(entity, eNsTagComp::MainTileMapTag{});

	ecs.addComponent(entity,
		gNsCollComp::CollisionMaskComponent{
			.selfLayer = gNsCollData::Layer::Tile,
			.collidesWithMask = static_cast<uint8_t>(
				gNsCollData::Layer::Player |
				gNsCollData::Layer::Enemy
				)});

	return entity;
}

//Game::Actor::Map::TileMapActor::TileMapActor(eNsECS::EntityMgr& ecs)
//{
//	eNsECS::Entity entity = ecs.createEntity();
//
//	eNsLogic2DComp::TileMapComponent tileMapComp;
//
//	// 配列の情報設定
//	tileMapComp.numCols = 10;
//	tileMapComp.numRows = 10;
//
//	// その他の情報
//	tileMapComp.tileSize = 0.5f;
//	tileMapComp.origin = glm::vec2(tileMapComp.numCols * tileMapComp.tileSize * -0.5f, tileMapComp.numRows * tileMapComp.tileSize * -0.5f);
//
//	// 配列作成
//	tileMapComp.tiles.resize(tileMapComp.numRows, std::vector<eNsLogic2DComp::Tile>(tileMapComp.numCols));
//
//	ecs.addComponent(entity, tileMapComp);
//
//	eNsDebugLog::GeneralLog("TileMapActor.cpp", "Tilemap creation completed successfully");
//}