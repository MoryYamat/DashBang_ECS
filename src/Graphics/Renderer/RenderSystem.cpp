#include "RenderSystem.h"

#include "Core/ECS/Entity.h"


#include "Core/ECS/Component/MaterialComponent.h"

#include <iostream>

// Changed to update RenderContext
void RenderSystem::RenderSystem(ECS& ecs, Shader& shader, float aspect, RenderContext& context)
{
	// View行列、Projection行列
	glm::mat4 view, projection;

	if (!getCameraMatrices(ecs, view, projection, context))
	{
		std::cerr << "[RenderSystem.cpp]: No valid camera found in ECS." << std::endl;
		return;
	}



	for (Entity entity : ecs.view<TransformComponent, MeshComponent, MaterialComponent>())
	{
		auto& transformComp = ecs.get<TransformComponent>(entity);
		auto& meshComp = ecs.get<MeshComponent>(entity);
		auto& materialComp = ecs.get<MaterialComponent>(entity);

		// state machine (シェーダーを切り替えると、viewもprojectionもセットする必要あり。)
		shader.Use();
		shader.setMat4("model", transformComp.toMatrix());
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("uBaseColor", materialComp.baseColor);
		drawMesh(meshComp);
	}

	context.viewMatrix = view;
	context.projectionMatrix = projection;
	//context.viewport = {0, 0, WindowManager::GetWidth(), WindowManager::GetHeight()};
}

void RenderSystem::drawMesh(const MeshComponent& meshComp)
{

	for (size_t i = 0; i < meshComp.modelData.meshes.size(); ++i)
	{
		const auto& meshGPU = meshComp.modelGPU.meshesGPU[i];
		const auto& meshData = meshComp.modelData.meshes[i];

		glBindVertexArray(meshGPU.vao);

		if (meshData.hasIndices)
		{
			glDrawElements(GL_TRIANGLES, meshGPU.indexCount, GL_UNSIGNED_INT, 0);
			//std::cout << "[RenderSystem.cpp]: draw elements" << std::endl;// for debugging
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, meshGPU.indexCount);
			//std::cout << "[RenderSystem.cpp]: draw Arrays" << std::endl;// for debugging
		}
	}

	glBindVertexArray(0);

	//glBindVertexArray(meshComp.modelGPU.meshesGPU[0].vao);
	//if (meshComp.modelData.meshes[0].hasIndices)
	//{
	//	glDrawElements(GL_TRIANGLES, meshComp.modelGPU.meshesGPU[0].indexCount, GL_UNSIGNED_INT, 0);
	//	//std::cout << "[RenderSystem.cpp]: draw elements" << std::endl;
	//}
	//else
	//{
	//	glDrawArrays(GL_TRIANGLES, 0, meshComp.modelGPU.meshesGPU[0].indexCount);
	//	//std::cout << "[RenderSystem.cpp]: draw Arrays" << std::endl;
	//}

}

bool RenderSystem::getCameraMatrices(ECS& ecs, glm::mat4& view, glm::mat4& projection)
{
	TransformComponent* camTransformComp = nullptr;
	CameraComponent* camComp = nullptr;

	for (Entity entity : ecs.view<TransformComponent, CameraComponent>())
	{
		camTransformComp = &ecs.get<TransformComponent>(entity);
		camComp = &ecs.get<CameraComponent>(entity);
		break;
	}

	if (!camTransformComp || !camComp)
	{
		std::cerr << "[RenderSystem.cpp(getCameraMatrices)]: No Camera found!" << std::endl;
		return false;
	}

	view = computeViewMatrix(*camTransformComp, *camComp);
	projection = computeProjectionMatrix(camComp->fov, camComp->aspect, camComp->nearClip, camComp->farClip);

	return true;
}

bool RenderSystem::getCameraMatrices(ECS& ecs, glm::mat4& view, glm::mat4& projection, RenderContext& context)
{

	for (Entity entity : ecs.view<TransformComponent, CameraComponent>())
	{
		const auto& camTransformComp = ecs.get<TransformComponent>(entity);
		const auto& camComp = ecs.get<CameraComponent>(entity);

		view = computeViewMatrix(camTransformComp, camComp);
		projection = computeProjectionMatrix(camComp.fov, camComp.aspect, camComp.nearClip, camComp.farClip);

		context.cameraPosition = camTransformComp.position;
		context.cameraFront = camComp.front;
		context.cameraRight = camComp.right;
		context.cameraUp = camComp.up;

		return true;

	}


	// カメラが見つからなかった
	std::cerr << "[RenderSystem.cpp(getCameraMatrices)]: No Camera found!" << std::endl;

	return false;
}



glm::mat4 RenderSystem::computeViewMatrix(const TransformComponent& transformComp, const CameraComponent& cameraComp)
{
	glm::vec3 position = transformComp.position;
	return glm::lookAt(position, position + cameraComp.front, cameraComp.up);
}

glm::mat4 RenderSystem::computeProjectionMatrix(float fov, float aspect, float nearClip, float farClip)
{
	return glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
}



// For simple testing
void RenderSystem::RenderSystem(ECS& ecs, Shader& shader, float aspect)
{
	//for (auto entity : ecs.view<MeshComponent>())// for Test
	//{
	//	//auto& transformComp = ecs.get<TransformComponent>(entity);
	//	auto& meshComp = ecs.get<MeshComponent>(entity);
	// 
	//	drawMesh(meshComp);
	//}
	glm::mat4 view, projection;

	if (!getCameraMatrices(ecs, view, projection))
	{
		std::cerr << "[RenderSystem.cpp]: No valid camera found in ECS." << std::endl;
		return;
	}



	for (Entity entity : ecs.view<TransformComponent, MeshComponent>())
	{
		auto& transformComp = ecs.get<TransformComponent>(entity);
		auto& meshComp = ecs.get<MeshComponent>(entity);


		// state machine (シェーダーを切り替えると、viewもprojectionもセットする必要あり。)
		shader.Use();
		shader.setMat4("model", transformComp.toMatrix());
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		drawMesh(meshComp);
	}

}