// player position control system
#pragma once

#include "Core/ECS/EntityManager.h"
#include "Game/Input/InputState.h"

#include "Core/InputManager/RawInputState.h"

#include "Graphics/Renderer/RenderContext.h"

#include <glm/glm.hpp>

namespace PlayerCharacterControlSystem
{
	void Update(ECS& ecs, const RawInputState& rawInput, RenderContext& renderContext, float deltaTime);
	void Update(ECS& ecs, InputState& input, float deltaTime);
	void Update(ECS& ecs, InputState& input, float deltaTime, RenderContext& renderContext);

	// Calculate the position vector of the mouse pointer in logical coordinates with the character as the origin
	// 論理座標における、キャラクターを原点としたマウスポインタの位置ベクトルを計算

};