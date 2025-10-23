// player position control system
#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Game/Input/InputState.h"

// #include "Engine/InputManager/RawInputState.h"
#include "Engine/InputManager/Private/RawInputState.h"

#include "Engine/Graphics/Private/Renderer/RenderContext.h"

#include <glm/glm.hpp>



// semantic Layer導入後 廃止予定(Intentへ移行予定)
// semantic Layer導入後 廃止予定(Intentへ移行予定)
// semantic Layer導入後 廃止予定(Intentへ移行予定)
namespace Game::Input::Player
{

	// 昔のやつ
	void Update(Engine::ECS::EntityMgr& ecs, const Engine::Input::RawInputState& rawInput, Engine::Graphics::Render::RenderContext& renderContext, float deltaTime);
	void Update(Engine::ECS::EntityMgr& ecs, InputState& input, float deltaTime);
	void Update(Engine::ECS::EntityMgr& ecs, InputState& input, float deltaTime, Engine::Graphics::Render::RenderContext& renderContext);

	// Calculate the position vector of the mouse pointer in logical coordinates with the character as the origin
	// 論理座標における、キャラクターを原点としたマウスポインタの位置ベクトルを計算

};