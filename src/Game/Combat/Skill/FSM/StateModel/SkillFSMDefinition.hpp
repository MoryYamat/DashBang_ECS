// SkillPhase定義やFSM構造，状態型
#pragma once


#include <cstdint>

#include "Game/Combat/Skill/FSM/StateModel/SkillTransitionTable.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMStates.hpp"

#include "Game/Combat/Skill/FSM/Effect/Hook/SkillEffectHook.hpp"

#include "Game/Combat/Skill/FSM/Reset/ResetHookDefinition.hpp"

#include <typeindex>

namespace Game::Combat::Skill::FSM
{
	using namespace Game::Combat::Skill::FSM::Effect;

	using namespace Game::Combat::Skill::FSM::Reset;

	struct SkillFSMDefinition
	{
		uint32_t id;
		std::vector<SkillTransition> transitions;
		std::type_index initialState = typeid(SkillPhase::Casting);// 明示的に初期状態を定義

		std::vector<SkillEffectHook> effectHooks;
		std::vector<SkillFSMResetHook> resetHooks;
	};
}