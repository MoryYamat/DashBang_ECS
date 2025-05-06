// Debug drawing functions
#pragma once

#include "Core/ECS/EntityManager.h"
#include "Graphics/Renderer/RenderContext.h"

namespace LogicDebugDrawSystem
{
	void Draw(ECS& ecs, const RenderContext& renderContext);

	void DebugDrawLogicPlayerPositions(ECS& ecs, const RenderContext& renderContext);

	void DebugDrawLogicTileMaps(ECS& ecs, const RenderContext& renderContext);

	void SetOpenGLMatrixState(const RenderContext& renderContext);

	void ResetOpenGLMatrixState();
}