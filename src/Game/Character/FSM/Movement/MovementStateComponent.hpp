#pragma once 

#include "MovementFSMDefinition.hpp"

#include <typeindex>

namespace Game::Character::FSM::Movement
{
	struct MovementStateComponent
	{
		std::type_index current; // 現在の状態を表す型のインデックス

		MovementStateComponent() : current(typeid(MovementState::Idle)) {} // 無効状態はvoid型

		explicit MovementStateComponent(std::type_index init)
			: current(init) {} // 初期状態を指定して初期化
	};
}