// Mapping InputAction and SkillId
// InputActionとSkillIdのマッピング

#pragma once

#include <unordered_map>
#include "Game/Input/InputAction.h"

#include "Game/SkillSystem/MasterData/SkillSlot.h"

class SkillInputMap
{
public:
	void bind(InputAction action, SkillSystem::SkillSlot slot)
	{
		mapping[action] = slot;
	}

	bool hasBinding(InputAction action) const
	{
		return mapping.find(action) != mapping.end();
	}

	SkillSystem::SkillSlot getSkillID(InputAction action) const
	{
		auto it = mapping.find(action);
		if (it != mapping.end())
			return it->second;
		else
			return SkillSystem::SkillSlot::None;// 無効ID
	}

	const std::unordered_map<InputAction, SkillSystem::SkillSlot>& getAllBindings() const
	{
		return mapping;
	}

private:
	std::unordered_map<InputAction, SkillSystem::SkillSlot> mapping;
};