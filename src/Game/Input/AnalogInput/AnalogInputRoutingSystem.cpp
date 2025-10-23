#include "AnalogInputRoutingSystem.h"

#include "Engine/ECS/Component/Input/AnalogInputComponent.h"

#include "Game/Utils/SpatialTransformUtils.h"

#include "Engine/InputManager/Private/RawInputState.h"


#include "Engine/Debug/Private/DebugUtils.h"

void Game::Input::Analog::RouteAnalogInput(Engine::ECS::EntityMgr& ecs, const Engine::Input::RawInputState& rawInput, const Engine::Graphics::Render::RenderContext& renderContext)
{
	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Input::AnalogInputComponent>())
	{
		auto& analog = ecs.get<Engine::ECS::Component::Input::AnalogInputComponent>(e);

		// マウスのスクリーン座標からXZ平面への投影位置の計算
		analog.cursorLogicPositionXZ = Game::Utils::ProjectScreenToLogicXZPlane(rawInput.mousePosition, renderContext);
		analog.cursorDelta = rawInput.mouseDelta;
		analog.scrollDelta = rawInput.scrollDelta;

		// Engine::Debug::Logging::LogVector("AnalogInputRoutingSystem.cpp(Route)", analog.cursorLogicPositionXZ);
	}
}