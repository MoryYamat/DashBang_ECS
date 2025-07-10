#pragma once

#include "Game/Combat/Skill/FSM/Definition/SkillFSMConditions.hpp"

#include <typeindex>

#include <memory>
#include <vector>

#include <optional>

#include "Common/GameNamespaceDecl.h"

namespace Game::Combat::Skill::FSM
{
	struct SkillTransition
	{
		std::optional<std::type_index> from;// nullopt => ‚·‚×‚Ä‚Ìó‘Ô‚©‚ç
		std::type_index to;
		std::shared_ptr<gNsSkillFSM::ISkillFSMCondition> condition;
	};
}