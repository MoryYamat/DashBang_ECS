#pragma once
#include "Game/Character/Private/Input/Private/InputAction.hpp"

#include <unordered_map>
#include <optional>
namespace Game::Character::Input
{
	// 生入力->抽象化ボタンの対応関係の情報を保持
	class RawToInputAction
	{
	public:
		void bindKey(int keyCode, InputAction action)
		{
			keyToAction[keyCode] = action;
		}

		std::optional<InputAction> getAction(int keyCode) const 
		{
			auto it = keyToAction.find(keyCode);
			if (it != keyToAction.end()) return it->second;
			return std::nullopt;
		}

	private:

		std::unordered_map<int, InputAction> keyToAction;
	};


}