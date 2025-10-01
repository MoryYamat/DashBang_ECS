// test object (ECS logical data chunks)
#include "TriangleActor.h"

#include <vector>
#include <iostream>

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Graphics/MeshComponent.h"

#include "Engine/Graphics/Model/ModelData.h"

#include "Engine/Graphics/Renderer/GPUBufferUtils.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include "Common/GameNamespaceDecl.h"

Test::Actor::TriangleActor::TriangleActor(eNsECS::EntityMgr& ecs)
{
	namespace Ops = Engine::ECS::Ops;
	namespace Comp = Engine::ECS::Component;

	eNsECS::Entity e = ecs.createEntity();

	std::vector < eNsGfxModel::VertexData > vertices = {
	{ {0.0f,  0.5f, 0.0f} },
	{ {-0.5f, -0.5f, 0.0f} },
	{ {0.5f, -0.5f, 0.0f} }
	};

	std::vector<unsigned int> indices = { 0, 1, 2 };

	eNsGfxModel::MeshData meshData;
	eNsGfxModel::ModelData modelData;
	meshData.vertices = vertices;
	meshData.indices = indices;
	meshData.hasIndices = true;
	modelData.meshes.push_back(std::move(meshData));

	eNsGfxModel::ModelGPU modelGPU = eNsGfxRender::GPUBufferUtils::createMeshGPUBuffers(modelData);

	Ops::Add<Comp::Graphics::MeshComponent>(ecs, e,
		Comp::Graphics::MeshComponent
		{
			std::move(modelData),
			std::move(modelGPU)
		});

	std::cout << "[TriangleActor.cpp]: Triangle generated" << std::endl;
}
