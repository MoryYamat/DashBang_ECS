#include "TestBaseTerrainActor.h"

#include "Engine/ECS/Entity.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Graphics/MeshComponent.h"
#include "Engine/ECS/Component/Graphics/ShaderComponent.h"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"

// test color
#include "Engine/ECS/Component/Graphics/MaterialComponent.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Engine/ECS/Component/Tags/MapTags.h"

#include "Game/Init/InitModel/InitLogicTransformFromModel.h"
#include "Game/Init/InitTileMap/InitTileMap.h"

#include "Engine/Graphics/Model/ModelData.h"

#include "Engine/Graphics/Model/AssimpImporter.h"
#include "Engine/Graphics/Renderer/GPUBufferUtils.h"

#include "Engine/Math/Logic2D/LogicMathUtils.h"

#include "Engine/Debug/DebugUtils.h"

#include "Game/Actor/Map/TileMapActor.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <iostream>

Game::Actor::Map::TestBaseTerrainActor::TestBaseTerrainActor(eNsECS::EntityMgr& ecs, eNsGfxRender::Shader* shader)
{
	namespace Ops = Engine::ECS::Ops;
	namespace Component = Engine::ECS::Component;

	eNsECS::Entity entity = ecs.createEntity();

	// モデルデータインポート
	eNsGfxModel::ModelData modelData = eNsGfxModel::AssimpImporter::Import("Assets/Models/BaseMesh.fbx");
	//ModelData modelData = AssimpImporter::Import("Assets/Models/HorizontallyTerrainMesh.fbx");
	for (const auto& mesh : modelData.meshes)
	{
		std::cout << "[PlayerCharacterActor.cpp]: Vertices: " << mesh.vertices.size()
			<< ", Indices: " << mesh.indices.size()
			<< ", hasIndices: " << mesh.hasIndices << std::endl;
	}

	// GPUBufferをインポートデータから作成
	eNsGfxModel::ModelGPU modelGPU = eNsGfxRender::GPUBufferUtils::createMeshGPUBuffers(modelData);
	Ops::Add<Component::Graphics::MeshComponent>(ecs, entity,
		Component::Graphics::MeshComponent{ std::move(modelData), std::move(modelGPU)}
		);


	// 初期描画座標を設定
	eNsCommonComp::TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.rotation = glm::vec3(0.0f, 30.0f, 0.0f);
	transformComp.scale = glm::vec3(0.01f);
	Ops::Add<Component::Common::TransformComponent>(ecs, entity, transformComp);

	// set ShaderComponent
	eNsGfxComp::ShaderComponent shaderComp;
	shaderComp.shader = shader;
	if (shaderComp.shader)
	{
		shaderComp.shader->Use();
		shaderComp.shader->setMat4("model", transformComp.toMatrix());
		std::cout << "[TestBaseTerrainActor.cpp]: The model matrix was set successfully." << std::endl;
	}
	else
	{
		std::cout << "[TestBaseTerrainActor.cpp]: Shader not found." << std::endl;
	}
	Ops::Add<Component::Graphics::ShaderComponent>(ecs, entity, shaderComp);

	// 色情報を設定(デバッグ用)
	eNsGfxComp::MaterialComponent materialComp;
	materialComp.baseColor = glm::vec3(0.6f, 0.8f, 0.7f);
	eNsDebugLog::LogVector("TestBaseTerrainActor.cpp(Color)", materialComp.baseColor);
	Ops::Add<Component::Graphics::MaterialComponent>(ecs, entity, materialComp);

	// 描画情報から論理情報を初期化
	eNsLogic2DComp::Logic2DTransformComponent logic2DComp;
	logic2DComp = gNsInit::Logic2D::InitLogic2DTransformFromModel(transformComp, modelData);
	Ops::Add<Component::Logic2D::Logic2DTransformComponent>(ecs, entity, logic2DComp);

	std::cout << "TestBaseTerrainActor.cpp: Rotation " << logic2DComp.rotation << std::endl;


	// 削除予定
//	// 1.0f -> 1.0m (想定)
//	eNsLogic2DComp::TileMapComponent tileMapComp;
//	tileMapComp.tileSize = 0.5f;
//	tileMapComp = gNsInit::Logic2D::InitTileMapFromBounds(transformComp, modelData, logic2DComp, tileMapComp.tileSize);
//	// TileMapComponent内のtilesベクトルを作成(初期化)
//	gNsInit::Logic2D::InitTileMapTiles(tileMapComp);
////	GameInit::TileMapFromMesh::InitWalKableByTerrain(tileMapComp, modelData);
//	
//	// GameInit::TileMapFromMesh::ApplyObstacleCollidersToTileMap(ecs, tileMapComp);
//
//	ecs.addComponent(entity, tileMapComp);

	eNsLogic2DComp::CollisionComponent collisionComp;
	// calc world size on the xz plane
	glm::vec2 worldSize = gNsInit::Logic2D::GetModelXZSizeWithScale(transformComp, modelData);
	// calc world center on the xz plane
	glm::vec3 localCenter = modelData.GetCenter();
	glm::vec3 worldCenter3D = transformComp.toMatrix() * glm::vec4(localCenter, 1.0f);
	glm::vec2 worldCenterXZ = glm::vec2(worldCenter3D.x, worldCenter3D.z);
	// calc local vector axisX and axisZ
	float rotRad = logic2DComp.rotation;// 描画基準と論理基準の整合性を考える
	// Front = Z axis basis
	glm::vec2 axisZ = glm::normalize(eNsLogic2DMath::CalcForwardFromYaw((rotRad)));
	glm::vec2 axisX = eNsLogic2DMath::CalcRightFromYaw(rotRad);

	collisionComp.collider.shape = eNsLogic2DComp::Obb2D
	{
		.center = glm::vec2(0.0f),// ローカルオフセット
		.halfExtents = worldSize * 0.5f,
		.axisX = axisX,
		.axisZ = axisZ
	};
	Ops::Add<Component::Logic2D::CollisionComponent>(ecs, entity, collisionComp);

	Ops::Add<Component::Tags::TerrainMeshTag>(ecs, entity, Component::Tags::TerrainMeshTag{});

	// TileMapActorの作成(Entityとして)
	[[maybe_unused]] auto _ =  gNsActor::Map::TileMapActor::Create(ecs, transformComp, modelData, 0.5f);


	// 最終ログ
	eNsDebugLog::GeneralLog("TestBaseTerrainActor.cpp", "TestBaseTerrainActor creation completed successfully");
}

