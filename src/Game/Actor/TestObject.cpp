#include "TestObject.h"

#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityUtils/EntityUtils.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/MeshComponent.h"
#include "Core/ECS/Component/ShaderComponent.h"

#include "Core/ECS/Component/FollowCameraComponent.h"

// input
// #include "Core/ECS/Component/InputComponent.h"
#include "Core/ECS/Component/InputActionComponent.h"

// test color
#include "Core/ECS/Component/MaterialComponent.h"

// Flags
#include "Core/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Core/ECS/Component/NameComponent.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"


#include "DataTypes/ModelData.h"
#include "Graphics/Model/AssimpImporter.h"
#include "Graphics/Renderer/GPUBufferUtils.h"

#include "Debug/DebugUtils.h"

#include <iostream>

TestObject::TestObject(ECS& ecs, Shader* shader)
{
	Entity entity = ecs.createEntity();

	// load Model Datas from file
	ModelData modelData = AssimpImporter::Import("Assets/Models/Ch44_nonPBR.fbx");
	for (const auto& mesh : modelData.meshes)
	{
		std::cout << "[PlayerCharacterActor.cpp]: Vertices: " << mesh.vertices.size()
			<< ", Indices: " << mesh.indices.size()
			<< ", hasIndices: " << mesh.hasIndices << std::endl;
	}

	// set Mesh data to GPUBuffers
	ModelGPU modelGPU = GPUBufferUtils::createMeshGPUBuffers(modelData);

	// set MeshComponent
	ecs.addComponent(entity, MeshComponent{
			modelData,
			modelGPU
		});


	// set TransformComponent
	TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, 0.0f);
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
		std::cout << "[PlayerCharacterActor.cpp]: The model matrix was set successfully." << std::endl;
	}
	else
	{
		std::cout << "[PlayerCharacterActor.cpp]: Shader not found." << std::endl;
	}
	ecs.addComponent(entity, shaderComp);

	// set Test Corlor
	MaterialComponent materialComp;
	materialComp.baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	ecs.addComponent(entity, materialComp);
}