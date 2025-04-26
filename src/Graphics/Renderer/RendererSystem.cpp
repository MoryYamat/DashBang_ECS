#include "RendererSystem.h"

#include <iostream>

void RenderSystem::RenderSystem(ECS& ecs, Shader& shader, float aspect)
{
	for (auto entity : ecs.view<MeshComponent>())
	{
		//auto& transformComp = ecs.get<TransformComponent>(entity);
		auto& meshComp = ecs.get<MeshComponent>(entity);

		drawMesh(meshComp);
	}
}

void RenderSystem::drawMesh(const MeshComponent& meshComp)
{

	glBindVertexArray(meshComp.modelGPU.meshesGPU[0].vao);
	if (meshComp.modelData.meshes[0].hasIndices)
	{
		glDrawElements(GL_TRIANGLES, meshComp.modelGPU.meshesGPU[0].indexCount, GL_UNSIGNED_INT, 0);
		//std::cout << "[RenderSystem.cpp]: draw elements" << std::endl;
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, meshComp.modelGPU.meshesGPU[0].indexCount);
		//std::cout << "[RenderSystem.cpp]: draw Arrays" << std::endl;
	}

	glBindVertexArray(0);
}