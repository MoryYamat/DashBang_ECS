#include "LogicDebugDrawSystem.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"

#include "Debug/DebugUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void LogicDebugDrawSystem::Draw(ECS& ecs, const RenderContext& renderContext)
{
	// view/projection を事前にセット
	// 固定機能パイプラインの古い機能
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(renderContext.projection));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(renderContext.view));

	for (Entity e : ecs.view<Logic2DTransformComponent>())
	{
		const auto& logic = ecs.get<Logic2DTransformComponent>(e);

		glm::vec3 worldPos(logic.positionXZ.x, 0.01f, logic.positionXZ.y);

		DebugUtils::DebugDraw::DrawCross(worldPos, 0.1f);
	}
}