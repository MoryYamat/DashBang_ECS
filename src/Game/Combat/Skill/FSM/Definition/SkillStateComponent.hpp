#pragma once


#include "Game/Combat/Skill/FSM/Definition/SkillFSMStates.hpp"

#include <typeindex>

#include "Common/GameNamespaceDecl.h"

namespace Game::Combat::Skill::FSM
{
	// スキルの状態を表すコンポーネント
	struct SkillStateComponent
	{
		std::type_index current; // 現在の状態を表す型のインデックス

		SkillStateComponent() : current(typeid(Game::Combat::Skill::FSM::SkillPhase::None)) {} // 初期状態はNone

		explicit SkillStateComponent(std::type_index init)
			: current(init) {} // 初期状態を指定して初期化
	};
}