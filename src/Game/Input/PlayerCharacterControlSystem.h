// player position control system
#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Game/Input/InputState.h"

#include "Engine/InputManager/RawInputState.h"

#include "Engine/Graphics/Renderer/RenderContext.h"

#include <glm/glm.hpp>

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"
// semantic Layer導入後 廃止予定(Intentへ移行予定)
// semantic Layer導入後 廃止予定(Intentへ移行予定)
// semantic Layer導入後 廃止予定(Intentへ移行予定)
namespace Game::Input::Player
{

	// 昔のやつ
	void Update(eNsECS::EntityMgr& ecs, const eNsInput::RawInputState& rawInput, eNsGfxRender::RenderContext& renderContext, float deltaTime);
	void Update(eNsECS::EntityMgr& ecs, InputState& input, float deltaTime);
	void Update(eNsECS::EntityMgr& ecs, InputState& input, float deltaTime, eNsGfxRender::RenderContext& renderContext);

	// Calculate the position vector of the mouse pointer in logical coordinates with the character as the origin
	// 論理座標における、キャラクターを原点としたマウスポインタの位置ベクトルを計算

};