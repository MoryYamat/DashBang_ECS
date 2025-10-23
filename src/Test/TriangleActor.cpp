// test object (ECS logical data chunks)
#include "TriangleActor.h"

#include <vector>
#include <iostream>

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Graphics/MeshComponent.h"

#include "Engine/Graphics/Private/Model/ModelData.h"

#include "Engine/Graphics/Private/Renderer/GPUBufferUtils.h"

#include "Engine/ECS/Ops/CoreOps.hpp"



Test::Actor::TriangleActor::TriangleActor(Engine::ECS::EntityMgr& ecs)
{
	namespace Ops = Engine::ECS::Ops;
	namespace Comp = Engine::ECS::Component;

	Engine::ECS::Entity e = ecs.createEntity();

	std::vector < Engine::Graphics::Model::VertexData > vertices = {
	{ {0.0f,  0.5f, 0.0f} },
	{ {-0.5f, -0.5f, 0.0f} },
	{ {0.5f, -0.5f, 0.0f} }
	};

	std::vector<unsigned int> indices = { 0, 1, 2 };

	Engine::Graphics::Model::MeshData meshData;
	Engine::Graphics::Model::ModelData modelData;
	meshData.vertices = vertices;
	meshData.indices = indices;
	meshData.hasIndices = true;
	modelData.meshes.push_back(std::move(meshData));

	Engine::Graphics::Model::ModelGPU modelGPU = Engine::Graphics::Render::GPUBufferUtils::createMeshGPUBuffers(modelData);

	Ops::Add<Comp::Graphics::MeshComponent>(ecs, e,
		Comp::Graphics::MeshComponent
		{
			std::move(modelData),
			std::move(modelGPU)
		});

	std::cout << "[TriangleActor.cpp]: Triangle generated" << std::endl;
}
