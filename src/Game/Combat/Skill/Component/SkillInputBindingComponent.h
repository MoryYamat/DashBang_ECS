//

#pragma once

#include "Common/GameNamespaceDecl.h"

#include "Game/Input/InputAction.h"
#include "Game/Combat/Skill/MasterData/SkillSlot.h"

#include <unordered_map>

namespace Game::Combat::Skill::Component
{
	struct SkillInputBindingComponent
	{
		std::unordered_map<gNsInput::InputAction, gNsSkillData::SkillSlot> actionToSlot;
	};
}