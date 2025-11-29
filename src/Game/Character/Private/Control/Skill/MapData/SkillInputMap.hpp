// Mapping InputAction and SkillId
// InputActionとSkillIdのマッピング

#pragma once

#include <unordered_map>

#include "Game/Input/Public/InputActionComponent.hpp"
#include "Game/Combat/Private/Skill/MasterData/SkillSlot.h"


namespace Game::Character::Control::Skill
{
	class SkillInputMap
	{
	public:
		void bind(Game::Input::InputAction action, Game::Combat::Skill::Data::SkillSlot slot)
		{
			mapping[action] = slot;
		}

		bool hasBinding(Game::Input::InputAction action) const
		{
			return mapping.find(action) != mapping.end();
		}

		Game::Combat::Skill::Data::SkillSlot getSkillID(Game::Input::InputAction action) const
		{
			auto it = mapping.find(action);
			if (it != mapping.end())
				return it->second;
			else
				return Game::Combat::Skill::Data::SkillSlot::None;// 無効ID
		}

		const std::unordered_map<Game::Input::InputAction, Game::Combat::Skill::Data::SkillSlot>& getAllBindings() const
		{
			return mapping;
		}

	private:
		std::unordered_map<Game::Input::InputAction, Game::Combat::Skill::Data::SkillSlot> mapping;
	};
}