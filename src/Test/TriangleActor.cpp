// test object (ECS logical data chunks)
#include "TriangleActor.h"

#include <vector>
#include <iostream>

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Graphics/MeshComponent.h"

#include "Engine/Graphics/Model/ModelData.h"

#include "Engine/Graphics/Renderer/GPUBufferUtils.h"

#include "Common/GameNamespaceDecl.h"

Test::Actor::TriangleActor::TriangleActor(eNsECS::EntityMgr& ecs)
{
	eNsECS::Entity entity = ecs.createEntity();

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
	modelData.meshes.push_back(meshData);

	eNsGfxModel::ModelGPU modelGPU = eNsGfxRender::GPUBufferUtils::createMeshGPUBuffers(modelData);

	ecs.addComponent(entity, eNsGfxComp::MeshComponent{
			modelData,
			modelGPU
		});

	std::cout << "[TriangleActor.cpp]: Triangle generated" << std::endl;
}
