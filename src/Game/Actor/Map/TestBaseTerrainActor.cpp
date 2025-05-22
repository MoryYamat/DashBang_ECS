#include "TestBaseTerrainActor.h"

#include "Core/ECS/Entity.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/MeshComponent.h"
#include "Core/ECS/Component/ShaderComponent.h"

#include "Core/ECS/Component/Collision/CollisionComponent.h"

// test color
#include "Core/ECS/Component/MaterialComponent.h"

#include "Core/ECS/Component/TileMapComponent.h"

#include "Game/Init/InitModel/InitLogicTransformFromModel.h"
#include "Game/Init/InitTileMap/InitTileMap.h"

#include "DataTypes/ModelData.h"

#include "Graphics/Model/AssimpImporter.h"
#include "Graphics/Renderer/GPUBufferUtils.h"

#include "Debug/DebugUtils.h"

#include <iostream>

TestBaseTerrainActor::TestBaseTerrainActor(ECS& ecs, Shader* shader)
{
	Entity entity = ecs.createEntity();

	// モデルデータインポート
	ModelData modelData = AssimpImporter::Import("Assets/Models/BaseMesh.fbx");
	//ModelData modelData = AssimpImporter::Import("Assets/Models/HorizontallyTerrainMesh.fbx");
	for (const auto& mesh : modelData.meshes)
	{
		std::cout << "[PlayerCharacterActor.cpp]: Vertices: " << mesh.vertices.size()
			<< ", Indices: " << mesh.indices.size()
			<< ", hasIndices: " << mesh.hasIndices << std::endl;
	}

	// GPUBufferをインポートデータから作成
	ModelGPU modelGPU = GPUBufferUtils::createMeshGPUBuffers(modelData);
	ecs.addComponent(entity, MeshComponent{
			modelData,
			modelGPU
		});

	// 初期描画座標を設定
	TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.rotation = glm::vec3(0.0f, 30.0f, 0.0f);
	transformComp.scale = glm::vec3(0.01f);
	ecs.addComponent(entity, transformComp);

	// set ShaderComponent
	ShaderComponent shaderComp;
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
	ecs.addComponent(entity, shaderComp);

	// 色情報を設定(デバッグ用)
	MaterialComponent materialComp;
	materialComp.baseColor = glm::vec3(0.6f, 0.8f, 0.7f);
	DebugUtils::LogVector("TestBaseTerrainActor.cpp(Color)", materialComp.baseColor);
	ecs.addComponent(entity, materialComp);

	// 描画情報から論理情報を初期化
	Logic2DTransformComponent logic2DComp;
	logic2DComp = GameInit::LogicTransform::InitLogic2DTransformFromModel(transformComp, modelData);
	ecs.addComponent(entity, logic2DComp);

	std::cout << "TestBaseTerrainActor.cpp: Rotation " << logic2DComp.rotation << std::endl;

	// 1.0f -> 1.0m (想定)
	TileMapComponent tileMapComp;
	tileMapComp.tileSize = 0.5f;
	tileMapComp = GameInit::TileMapFromMesh::InitTileMapFromBounds(transformComp, modelData, logic2DComp, tileMapComp.tileSize);
	// TileMapComponent内のtilesベクトルを作成(初期化)
	GameInit::TileMapFromMesh::InitTileMapTiles(tileMapComp);
//	GameInit::TileMapFromMesh::InitWalKableByTerrain(tileMapComp, modelData);
	
	// GameInit::TileMapFromMesh::ApplyObstacleCollidersToTileMap(ecs, tileMapComp);

	ecs.addComponent(entity, tileMapComp);

	CollisionComponent collisionComp;
	// calc world size on the xz plane
	collisionComp.collider.type = ColliderType::Obb2D;
	collisionComp.collider.obb2D.center = logic2DComp.positionXZ;
	glm::vec2 worldSize = GameInit::LogicTransform::GetModelXZSizeWithScale(transformComp, modelData);
	collisionComp.collider.obb2D.halfExtents = worldSize * 0.5f;

	// calc world center on the xz plane
	glm::vec3 localCenter = modelData.GetCenter();
	glm::vec3 worldCenter3D = transformComp.toMatrix() * glm::vec4(localCenter, 1.0f);
	glm::vec2 worldCenterXZ = glm::vec2(worldCenter3D.x, worldCenter3D.z);
	collisionComp.collider.obb2D.center = worldCenterXZ;

	// calc local vector axisX and axisZ
	// float rotRad = glm::radians(-logic2DComp.rotation);// OpenGLは右手系だが，logic.rotationは右回りとして格納されているので，符号を反転
	float rotRad = -logic2DComp.rotation;// OpenGLは右手系だが，logic.rotationは右回りとして格納されているので，符号を反転
	glm::vec2 axisX = glm::normalize(glm::vec2(std::cos(rotRad), std::sin(rotRad)));
	glm::vec2 axisZ = glm::vec2(-axisX.y, axisX.x);
	collisionComp.collider.obb2D.axisX = axisX;
	collisionComp.collider.obb2D.axisZ = axisZ;
	ecs.addComponent(entity, collisionComp);


	// 最終ログ
	DebugUtils::GeneralLog("TestBaseTerrainActor.cpp", "TestBaseTerrainActor creation completed successfully");
}

