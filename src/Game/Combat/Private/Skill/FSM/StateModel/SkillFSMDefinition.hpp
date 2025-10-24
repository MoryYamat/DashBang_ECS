// SkillPhase定義やFSM構造，状態型
#pragma once


#include <cstdint>

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillTransitionTable.hpp"
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMStates.hpp"

#include "Game/Combat/Private/Skill/FSM/Effect/Hook/SkillEffectHook.hpp"

#include "Game/Combat/Private/Skill/FSM/Reset/ResetHookDefinition.hpp"

#include <typeindex>

namespace Game::Combat::Skill::FSM
{

	struct SkillFSMDefinition
	{
		uint32_t id;
		std::vector<SkillTransition> transitions;
		std::type_index initialState = typeid(SkillPhase::Casting);// 明示的に初期状態を定義

		std::vector<Game::Combat::Skill::FSM::Effect::SkillEffectHook> effectHooks;
		std::vector<Game::Combat::Skill::FSM::Reset::SkillFSMResetHook> resetHooks;
	};
}