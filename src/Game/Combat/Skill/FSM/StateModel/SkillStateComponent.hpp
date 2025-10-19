#pragma once


#include "Game/Combat/Skill/FSM/StateModel/SkillFSMStates.hpp"

#include <typeindex>



namespace Game::Combat::Skill::FSM::StateModel
{
	// スキルの状態を表すコンポーネント
	struct SkillStateComponent
	{
		std::type_index current; // 現在の状態を表す型のインデックス
		std::type_index previous;

		SkillStateComponent() : current(typeid(Game::Combat::Skill::FSM::SkillPhase::None)) , 
			previous(typeid(Game::Combat::Skill::FSM::SkillPhase::None))
		{} // 初期状態はNone

		explicit SkillStateComponent(std::type_index init, std::type_index prev)
			: current(init) , previous(prev) {} // 初期状態を指定して初期化
	};
}