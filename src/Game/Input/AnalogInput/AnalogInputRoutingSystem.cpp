#include "AnalogInputRoutingSystem.h"

#include "Engine/ECS/Component/Input/AnalogInputComponent.h"

#include "Game/Utils/SpatialTransformUtils.h"

#include "Common/EngineNamespaceDecl.h"

#include "Engine/Debug/DebugUtils.h"

void Game::Input::Analog::RouteAnalogInput(eNsECS::EntityMgr& ecs, const eNsInput::RawInputState& rawInput, const eNsGfxRender::RenderContext& renderContext)
{
	for (eNsECS::Entity e : ecs.view<eNsInputComp::AnalogInputComponent>())
	{
		auto& analog = ecs.get<eNsInputComp::AnalogInputComponent>(e);

		// マウスのスクリーン座標からXZ平面への投影位置の計算
		analog.cursorLogicPositionXZ = Game::Utils::ProjectScreenToLogicXZPlane(rawInput.mousePosition, renderContext);
		analog.cursorDelta = rawInput.mouseDelta;
		analog.scrollDelta = rawInput.scrollDelta;

		// eNsDebugLog::LogVector("AnalogInputRoutingSystem.cpp(Route)", analog.cursorLogicPositionXZ);
	}
}