// Rendering system

#pragma once

#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/MeshComponent.h"
#include "Core/ECS/Component/CameraComponent.h"

#include "Core/ECS/Component/FollowCameraComponent.h"

#include "Graphics/Renderer/Shader.h"
#include "Graphics/Renderer/RenderContext.h"
namespace RenderSystem
{
	void RenderSystem(ECS& ecs, Shader& shader, float aspect);
	void RenderSystem(ECS& ecs, Shader& shader, float aspect, RenderContext& context);


	void drawMesh(const MeshComponent& meshComp);


	bool getCameraMatrices(ECS& ecs, glm::mat4& view, glm::mat4& projection);
	bool getCameraMatrices(ECS& ecs, glm::mat4& view, glm::mat4& projection, RenderContext& context);

	glm::mat4 computeViewMatrix(const TransformComponent& transformComp, const CameraComponent& cameraComp);
	
	
	glm::mat4 computeViewMatrix(const TransformComponent& transformComp, const CameraComponent& cameraComp);

	glm::mat4 computeProjectionMatrix(float fov, float aspect, float nearClip, float farClip);

}
