// Entity�̃A�N�V������Ԃ�ێ�	
// Maintains the action state of the entity

#pragma once

#include "Game/Input/InputAction.h"

#include <unordered_map>

struct InputActionComponent
{
	// ���݂̃t���[���ŗL���ȃA�N�V����
	std::unordered_map<InputAction, bool> current;

	// 1�t���[���O�̏��(isJustPressed�Ȃ��̌��o�Ɏg��)
	std::unordered_map<InputAction, bool> previous;

	// ��������Ă邩
	bool isPressed(InputAction action) const
	{
		auto it = current.find(action);
		return it != current.end() && it->second;
	}

	// ���t���[���ŉ����ꂽ�u�Ԃ�?
	bool isJustPressed(InputAction action) const
	{
		return isPressed(action) && (!previous.count(action) || !previous.at(action));
	}

	// ���t���[���ŗ����ꂽ�u�Ԃ�
	bool isJustReleased(InputAction action) const
	{
		return !isPressed(action) && previous.count(action) && previous.at(action);
	}
};