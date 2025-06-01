#include "TestObject.h"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityUtils/EntityUtils.h"
#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Graphics/MeshComponent.h"
#include "Engine/ECS/Component/Graphics/ShaderComponent.h"

#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"

// input
// #include "Core/ECS/Component/InputComponent.h"
#include "Game/Input/InputActionComponent.h"

// test color
#include "Engine/ECS/Component/Graphics/MaterialComponent.h"

// Flags
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Utils/NameComponent.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"


#include "Engine/Graphics/Model/ModelData.h"
#include "Engine/Graphics/Model/AssimpImporter.h"
#include "Engine/Graphics/Renderer/GPUBufferUtils.h"

#include "Engine/Debug/DebugUtils.h"

#include <iostream>

Game::Actor::TestObject::TestObject(eNsECS::EntityMgr& ecs, eNsGfxRender::Shader* shader)
{
	eNsECS::Entity entity = ecs.createEntity();

	// load Model Datas from file
	eNsGfxModel::ModelData modelData = eNsGfxModel::AssimpImporter::Import("Assets/Models/Ch44_nonPBR.fbx");
	for (const auto& mesh : modelData.meshes)
	{
		std::cout << "[PlayerCharacterActor.cpp]: Vertices: " << mesh.vertices.size()
			<< ", Indices: " << mesh.indices.size()
			<< ", hasIndices: " << mesh.hasIndices << std::endl;
	}

	// set Mesh data to GPUBuffers
	eNsGfxModel::ModelGPU modelGPU = eNsGfxRender::GPUBufferUtils::createMeshGPUBuffers(modelData);

	// set MeshComponent
	ecs.addComponent(entity, eNsGfxComp::MeshComponent{
			modelData,
			modelGPU
		});


	// set TransformComponent
	eNsCommonComp::TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.rotation = glm::vec3(0.0f, 45.0f, 0.0f);
	transformComp.scale = glm::vec3(0.01f);
	ecs.addComponent(entity, transformComp);


	// set ShaderComponent
	eNsGfxComp::ShaderComponent shaderComp;
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
	eNsGfxComp::MaterialComponent materialComp;
	materialComp.baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	ecs.addComponent(entity, materialComp);
}