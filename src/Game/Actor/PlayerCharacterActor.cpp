#include "PlayerCharacterActor.h"

#include "Core/ECS/Entity.h"
#include "Core/Utils/EntityUtils.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/MeshComponent.h"
#include "Core/ECS/Component/ShaderComponent.h"

#include "Core/ECS/Component/NameComponent.h"

#include "DataTypes/ModelData.h"

#include "Graphics/Model/AssimpImporter.h"
#include "Graphics/Renderer/GPUBufferUtils.h"


#include <iostream>

PlayerCharacter::PlayerCharacter(ECS& ecs, Shader* shader)
{
	Entity entity = ecs.createEntity();

	// load Model Datas from file
	ModelData data = AssimpImporter::Import("Assets/Models/Ch44_nonPBR.fbx");
	for (const auto& mesh : data.meshes)
	{
		std::cout << "[Test3DModel.cpp]: Vertices: " << mesh.vertices.size()
			<< ", Indices: " << mesh.indices.size()
			<< ", hasIndices: " << mesh.hasIndices << std::endl;
	}

	// set Mesh data to GPUBuffers
	ModelGPU modelGPU = GPUBufferUtils::createMeshGPUBuffers(data);

	// set MeshComponent
	ecs.addComponent(entity, MeshComponent{
			data,
			modelGPU
		});


	// set TransformComponent
	TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, -10.0f);
	transformComp.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.scale = glm::vec3(0.01f);
	ecs.addComponent(entity, transformComp);


	// set ShaderComponent
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
	ecs.addComponent(entity, shaderComp);

	// Set NameComponent
	NameComponent nameComp;
	nameComp.name = "Player";
	ecs.addComponent(entity, nameComp);


	std::cout << "[Test3Dmodel.cpp]: Test3Dmodel Settings Completed" << std::endl;


}

PlayerCharacter::~PlayerCharacter()
{

}