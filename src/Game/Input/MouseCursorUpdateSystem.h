// A function that updates the mouse cursor state.

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Input/InputState.h"

#include "Engine/InputManager/RawInputState.h"

#include "Engine/Graphics/Renderer/RenderContext.h"




namespace Game::Input::Analog
{
	// 現在未使用：意図層導入後未使用 (タイトル画面などゲーム外のアナログ入力反映のため))
	void Update(Engine::ECS::EntityMgr& ecs, const Engine::Input::RawInputState& rawInput, Engine::Graphics::Render::RenderContext& renderContext);

	// 削除予定
	void Update(Engine::ECS::EntityMgr& ecs, InputState& input, Engine::Graphics::Render::RenderContext renderContext);
}