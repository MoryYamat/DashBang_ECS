#include "MouseCursorUpdateSystem.h"

#include "Core/ECS/Entity.h"
#include "Core/ECS/Component/MouseCursorComponent.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"

#include "Game/Utils/SpatialTransformUtils.h"

#include "Debug/DebugUtils.h"

#include <glm/glm.hpp>

void MouseCursorUpdateSystem::Update(ECS& ecs, InputState& input, RenderContext renderContext)
{
	for (Entity e : ecs.view<MouseCursorComponent, Logic2DTransformComponent>())
	{
		auto& mouseComp = ecs.get<MouseCursorComponent>(e);
		auto& logic2DComp = ecs.get<Logic2DTransformComponent>(e);

		//mouseComp.logicPosition = GameUtils::SpatialTransform::ProjectScreenToLogicXZPlane(input.screenMousePosition, renderContext);
		//logic2DComp.positionXZ = mouseComp.logicPosition;

		logic2DComp.positionXZ = GameUtils::SpatialTransform::ProjectScreenToLogicXZPlane(input.screenMousePosition, renderContext);


		// DebugUtils::LogVector_string("MouseCursorUpdateSystem.cpp(mouseLogicPos)", mouseComp.logicPosition);
		// DebugUtils::LogVector_string("MouseCursorUpdateSystem.cpp(logic2DPos)", logic2DComp.positionXZ);

		break;
	}
}