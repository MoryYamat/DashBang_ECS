#include "SetInputMap.h"

#include "Game/Input/InputAction.h"
#include "Game/Input/InputMapping.h"

#include "Common/GameNamespaceDecl.h"

void Game::Input::Setting::InitInputMap(eNsECS::EntityMgr& ecs)
{
	auto& map = ecs.createResource<InputMapping>();

	map.bindKey(GLFW_KEY_W, gNsInput::InputAction::MoveForward);
	map.bindKey(GLFW_KEY_S, gNsInput::InputAction::MoveBackward);
	map.bindKey(GLFW_KEY_D, gNsInput::InputAction::MoveRight);
	map.bindKey(GLFW_KEY_A, gNsInput::InputAction::MoveLeft);
	map.bindKey(GLFW_MOUSE_BUTTON_1, gNsInput::InputAction::CastSkill1);
	map.bindKey(GLFW_MOUSE_BUTTON_2, gNsInput::InputAction::CastSkill2);
	map.bindKey(GLFW_KEY_1, gNsInput::InputAction::CastSkill3);
}