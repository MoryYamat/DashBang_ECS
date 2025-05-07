#include "TestBaseTerrainActor.h"

#include "Core/ECS/Entity.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/MeshComponent.h"
#include "Core/ECS/Component/ShaderComponent.h"

// test color
#include "Core/ECS/Component/MaterialComponent.h"

#include "Core/ECS/Component/TileMapComponent.h"

#include "Game/Init/InitLogicTransformFromModel.h"

#include "DataTypes/ModelData.h"

#include "Graphics/Model/AssimpImporter.h"
#include "Graphics/Renderer/GPUBufferUtils.h"

#include "Debug/DebugUtils.h"

#include <iostream>

TestBaseTerrainActor::TestBaseTerrainActor(ECS& ecs, Shader* shader)
{
	Entity entity = ecs.createEntity();

	// ���f���f�[�^�C���|�[�g
	ModelData modelData = AssimpImporter::Import("Assets/Models/BaseMesh.fbx");
	for (const auto& mesh : modelData.meshes)
	{
		std::cout << "[PlayerCharacterActor.cpp]: Vertices: " << mesh.vertices.size()
			<< ", Indices: " << mesh.indices.size()
			<< ", hasIndices: " << mesh.hasIndices << std::endl;
	}

	// GPUBuffer���C���|�[�g�f�[�^����쐬
	ModelGPU modelGPU = GPUBufferUtils::createMeshGPUBuffers(modelData);
	ecs.addComponent(entity, MeshComponent{
			modelData,
			modelGPU
		});

	// �����`����W��ݒ�
	TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, 00.0f);
	transformComp.rotation = glm::vec3(0.0f, 45.0f, 0.0f);
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

	// �F����ݒ�(�f�o�b�O�p)
	MaterialComponent materialComp;
	materialComp.baseColor = glm::vec3(0.6f, 0.8f, 0.7f);
	DebugUtils::LogVector("TestBaseTerrainActor.cpp(Color)", materialComp.baseColor);
	ecs.addComponent(entity, materialComp);

	// �`���񂩂�_������������
	Logic2DTransformComponent logic2DComp;
	logic2DComp = GameUtils::Init::InitLogic2DTransformFromModel(transformComp, modelData);
	ecs.addComponent(entity, logic2DComp);

	std::cout << "TestBaseTerrainActor.cpp: Rotation " << logic2DComp.rotation << std::endl;

	// 1.0f -> 1.0m (�z��)
	TileMapComponent tileMapComp;
	tileMapComp.tileSize = 0.5f;
	tileMapComp = GameUtils::Init::InitTileMapFromBounds(transformComp, modelData, logic2DComp, tileMapComp.tileSize);
	// TileMapComponent����tiles�x�N�g�����쐬(������)
	GameUtils::Init::InitTileMapTiles(tileMapComp);
	ecs.addComponent(entity, tileMapComp);

	// �ŏI���O
	DebugUtils::GeneralLog("TestBaseTerrainActor.cpp", "TestBaseTerrainActor creation completed successfully");
}