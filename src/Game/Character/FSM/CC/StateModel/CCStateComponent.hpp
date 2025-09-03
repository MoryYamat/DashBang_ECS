#pragma once

#include "Game/Character/FSM/CC/CCStateTags.hpp"
#include <typeindex>

namespace Game::Character::FSM::CC::StateModel
{
	// TODO: typeidのパフォーマンス問題
	struct CCStateComponent
	{
		std::type_index previous;
		std::type_index current; // 現在の状態を表す型のインデックス

		std::type_index issuerId;// typeid(SkillFSMAxis), typeid(CCAxis), etc.

		float enteredAt = 0.0f;

		CCStateComponent() : current(StateTag::NONE), previous(StateTag::NONE) , issuerId(AxisTag::CCAxis), enteredAt(0.0f){} // 無効状態はvoid型

		explicit CCStateComponent(std::type_index iniState, std::type_index mainAxis)
			: current(iniState), previous(iniState) , issuerId(mainAxis) {
		} // 初期状態を指定して初期化
	};
}