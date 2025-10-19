#include "Test3DModel.h"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Graphics/MeshComponent.h"
#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Graphics/ShaderComponent.h"

#include "Engine/Graphics/Model/ModelData.h"

#include "Engine/Graphics/Model/AssimpImporter.h"

#include "Engine/Graphics/Renderer/Shader.h"
#include "Engine/Graphics/Renderer/GPUBufferUtils.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <iostream>

Test::Actor::Test3DModel::Test3DModel(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Render::Shader* shader)
{
	namespace Ops = Engine::ECS::Ops;
	namespace Comp = Engine::ECS::Component;
	Engine::ECS::Entity e = ecs.createEntity();

	Engine::Graphics::Model::ModelData data = Engine::Graphics::Model::AssimpImporter::Import("Assets/Models/Ch44_nonPBR.fbx");

	for (const auto& mesh : data.meshes)
	{
		std::cout << "[Test3DModel.cpp]: Vertices: " << mesh.vertices.size()
			<< ", Indices: " << mesh.indices.size()
			<< ", hasIndices: " << mesh.hasIndices << std::endl;
	}

	Engine::Graphics::Model::ModelGPU modelGPU = Engine::Graphics::Render::GPUBufferUtils::createMeshGPUBuffers(data);

	Engine::ECS::Component::Common::TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, -10.0f);
	transformComp.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.scale = glm::vec3(0.01f);

	Engine::ECS::Component::Graphics::ShaderComponent shaderComp;

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


	Ops::Add<Comp::Graphics::MeshComponent>(ecs, e,
		Comp::Graphics::MeshComponent{
			std::move(data),
			std::move(modelGPU)
		});

	Ops::Add<Comp::Common::TransformComponent>(ecs, e, transformComp);

	Ops::Add<Comp::Graphics::ShaderComponent>(ecs, e, shaderComp);

	std::cout << "[Test3Dmodel.cpp]: Test3Dmodel Settings Completed" << std::endl;
}

Test::Actor::Test3DModel::~Test3DModel()
{
	
}