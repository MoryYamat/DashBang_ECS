#include "MouseCursorUpdateSystem.h"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/Component/Input/MouseCursorComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Game/Utils/SpatialTransformUtils.h"

#include "Engine/Debug/DebugUtils.h"

#include <glm/glm.hpp>


// 現在未使用：意図層導入後未使用 (今後使用可能性あり，(タイトル画面などゲーム外のアナログ入力反映のため))
void Game::Input::Analog::Update(Engine::ECS::EntityMgr& ecs, const Engine::Input::RawInputState& rawInput, Engine::Graphics::Render::RenderContext& renderContext)
{
	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Input::MouseCursorComponent, Engine::ECS::Component::Logic2D::Logic2DTransformComponent>())
	{
		auto& mouseComp = ecs.get<Engine::ECS::Component::Input::MouseCursorComponent>(e);
		auto& logic2DComp = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e);

		//mouseComp.logicPosition = GameUtils::SpatialTransform::ProjectScreenToLogicXZPlane(input.screenMousePosition, renderContext);
		//logic2DComp.positionXZ = mouseComp.logicPosition;

		logic2DComp.positionXZ = Game::Utils::ProjectScreenToLogicXZPlane(rawInput.mousePosition, renderContext);

		// position(0.0f, 0.0f)の時不具合が発生しないか、確認する
		// position(0.0f, 0.0f)の時不具合が発生しないか、確認する
		// position(0.0f, 0.0f)の時不具合が発生しないか、確認する
		// DebugUtils::LogVector_string("MouseCursorUpdateSystem.cpp(logic2DPos)", logic2DComp.positionXZ);

		break;
	}
}

// 昔のやつ (削除予定)
void Game::Input::Analog::Update(Engine::ECS::EntityMgr& ecs, InputState& input, Engine::Graphics::Render::RenderContext renderContext)
{
	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Input::MouseCursorComponent, Engine::ECS::Component::Logic2D::Logic2DTransformComponent>())
	{
		auto& mouseComp = ecs.get<Engine::ECS::Component::Input::MouseCursorComponent>(e);
		auto& logic2DComp = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e);

		//mouseComp.logicPosition = GameUtils::SpatialTransform::ProjectScreenToLogicXZPlane(input.screenMousePosition, renderContext);
		//logic2DComp.positionXZ = mouseComp.logicPosition;

		logic2DComp.positionXZ = Game::Utils::ProjectScreenToLogicXZPlane(input.screenMousePosition, renderContext);

		// position(0.0f, 0.0f)の時不具合が発生しないか、確認する
		// position(0.0f, 0.0f)の時不具合が発生しないか、確認する
		// position(0.0f, 0.0f)の時不具合が発生しないか、確認する
		// DebugUtils::LogVector_string("MouseCursorUpdateSystem.cpp(logic2DPos)", logic2DComp.positionXZ);

		break;
	}
}