// Entityのアクション状態を保持	
// Maintains the action state of the entity

#pragma once

#include "Game/Input/InputAction.h"

#include <unordered_map>

struct InputActionComponent
{
	// 現在のフレームで有効なアクション
	std::unordered_map<InputAction, bool> current;

	// 1フレーム前の状態(isJustPressedなｄの検出に使う)
	std::unordered_map<InputAction, bool> previous;

	// 今押されてるか
	bool isPressed(InputAction action) const
	{
		auto it = current.find(action);
		return it != current.end() && it->second;
	}

	// 今フレームで押された瞬間か?
	bool isJustPressed(InputAction action) const
	{
		return isPressed(action) && (!previous.count(action) || !previous.at(action));
	}

	// 今フレームで離された瞬間か
	bool isJustReleased(InputAction action) const
	{
		return !isPressed(action) && previous.count(action) && previous.at(action);
	}
};