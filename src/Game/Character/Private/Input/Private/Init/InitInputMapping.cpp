#include "Game/Character/Private/Input/Public/InputApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Game/Character/Private/Input/Private/InputMapping.hpp"

#include "Game/Character/Private/Input/Private/InputAction.hpp"

#include <GLFW/glfw3.h>

namespace Game::Character::Input
{
	void InitInputMapping(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& map = ctx.ww.CreateResource<RawToInputAction>();

		map.bindKey(GLFW_KEY_W, Game::Character::Input::InputAction::MoveForward);
		map.bindKey(GLFW_KEY_S, Game::Character::Input::InputAction::MoveBackward);
		map.bindKey(GLFW_KEY_D, Game::Character::Input::InputAction::MoveRight);
		map.bindKey(GLFW_KEY_A, Game::Character::Input::InputAction::MoveLeft);
		map.bindKey(GLFW_MOUSE_BUTTON_1, Game::Character::Input::InputAction::CastSkill1);
		map.bindKey(GLFW_MOUSE_BUTTON_2, Game::Character::Input::InputAction::CastSkill2);
		map.bindKey(GLFW_KEY_1, Game::Character::Input::InputAction::CastSkill3);
		map.bindKey(GLFW_KEY_2, Game::Character::Input::InputAction::CastSkill4);
	}
}