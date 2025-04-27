// test object (ECS logical data chunks)
#include "TriangleActor.h"

#include <vector>
#include <iostream>

#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Component/MeshComponent.h"

#include "DataTypes/ModelData.h"

#include "Graphics/Renderer/GPUBufferUtils.h"



TriangleActor::TriangleActor(ECS& ecs)
{
	Entity entity = ecs.createEntity();

	std::vector<VertexData> vertices = {
	{ {0.0f,  0.5f, 0.0f} },
	{ {-0.5f, -0.5f, 0.0f} },
	{ {0.5f, -0.5f, 0.0f} }
	};

	std::vector<unsigned int> indices = { 0, 1, 2 };

	MeshData meshData;
	ModelData modelData;
	meshData.vertices = vertices;
	meshData.indices = indices;
	meshData.hasIndices = true;
	modelData.meshes.push_back(meshData);

	ModelGPU modelGPU = GPUBufferUtils::createMeshGPUBuffers(modelData);

	ecs.addComponent(entity, MeshComponent{
			modelData,
			modelGPU
		});

	std::cout << "[TriangleActor.cpp]: Triangle generated" << std::endl;
}
