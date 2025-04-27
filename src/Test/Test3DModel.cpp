#include "Test3DModel.h"

#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Component/MeshComponent.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/ShaderComponent.h"

#include "DataTypes/ModelData.h"

#include "Graphics/Model/AssimpImporter.h"

#include "Graphics/Renderer/Shader.h"
#include "Graphics/Renderer/GPUBufferUtils.h"

#include <iostream>

Test3DModel::Test3DModel(ECS& ecs,Shader* shader)
{
	Entity entity = ecs.createEntity();

	ModelData data = AssimpImporter::Import("Assets/Models/Ch44_nonPBR.fbx");

	for (const auto& mesh : data.meshes)
	{
		std::cout << "[Test3DModel.cpp]: Vertices: " << mesh.vertices.size()
			<< ", Indices: " << mesh.indices.size()
			<< ", hasIndices: " << mesh.hasIndices << std::endl;
	}

	ModelGPU modelGPU = GPUBufferUtils::createMeshGPUBuffers(data);

	TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, -10.0f);
	transformComp.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.scale = glm::vec3(0.01f);

	ShaderComponent shaderComp;

	shaderComp.shader = shader;

	if (shaderComp.shader)
	{
		shaderComp.shader->Use();
		shaderComp.shader->setMat4("model", transformComp.toMatrix());
		std::cout << "[Test3DModel.cpp]: The model matrix was set successfully." << std::endl;
	}
	else
	{

	}



	ecs.addComponent(entity, MeshComponent{
			data,
			modelGPU
		});

	ecs.addComponent(entity, transformComp);

	ecs.addComponent(entity, shaderComp);

	std::cout << "[Test3Dmodel.cpp]: Test3Dmodel Settings Completed" << std::endl;
}

Test3DModel::~Test3DModel()
{
	
}