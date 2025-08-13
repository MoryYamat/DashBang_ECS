#pragma once

#include <typeindex>

namespace Game::Character::FSM::CC::StateModel
{
	struct CCStateComponent
	{
		std::type_index previous;
		std::type_index current; // 現在の状態を表す型のインデックス

		CCStateComponent() : current(StateTag::IDLE), previous(StateTag::IDLE) {} // 無効状態はvoid型

		explicit CCStateComponent(std::type_index init)
			: current(init), previous(init) {
		} // 初期状態を指定して初期化
	};
}