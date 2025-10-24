#pragma once

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMConditions.hpp"

#include <typeindex>

#include <memory>
#include <vector>

#include <optional>



namespace Game::Combat::Skill::FSM
{
	struct SkillTransition
	{
		std::optional<std::type_index> from;// nullopt => すべての状態から
		std::type_index to;
		std::shared_ptr<Game::Combat::Skill::FSM::ISkillFSMCondition> condition;
	};
}