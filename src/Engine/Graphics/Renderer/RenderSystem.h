// Rendering system

#pragma once

// Engine
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Graphics/MeshComponent.h"
#include "Engine/ECS/Component/Camera/CameraComponent.h"

#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"

#include "Engine/Graphics/Renderer/Shader.h"
#include "Engine/Graphics/Renderer/RenderContext.h"


// common
#include "Common/EngineNamespaceDecl.h"

namespace Engine::Graphics::Render
{
	void RenderSystem(eNsECS::EntityMgr& ecs, eNsGfxRender::Shader& shader, float aspect);
	void RenderSystem(eNsECS::EntityMgr& ecs, eNsGfxRender::Shader& shader, float aspect, RenderContext& context);


	void drawMesh(const eNsGfxComp::MeshComponent& meshComp);


	bool getCameraMatrices(eNsECS::EntityMgr& ecs, glm::mat4& view, glm::mat4& projection);
	bool getCameraMatrices(eNsECS::EntityMgr& ecs, glm::mat4& view, glm::mat4& projection, RenderContext& context);

	glm::mat4 computeViewMatrix(const eNsCommonComp::TransformComponent& transformComp, const eNsCamComp::CameraComponent& cameraComp);


	glm::mat4 computeViewMatrix(const eNsCommonComp::TransformComponent& transformComp, const eNsCamComp::CameraComponent& cameraComp);

	glm::mat4 computeProjectionMatrix(float fov, float aspect, float nearClip, float farClip);
}

