#pragma once

#include "Game/Character/FSM/Movement/StateModel/MovementFSMDefinition.hpp"

#include <string>
#include <unordered_map>

namespace Game::Character::FSM::Movement::Database
{
	using namespace Game::Character::FSM::Movement;

	struct MovementFSMDatabase
	{
		std::unordered_map<std::string, MovementFSMDefinition> definitions;

		void Add(const std::string& id, const MovementFSMDefinition& def)
		{
			if (definitions.count(id))
			{
				std::cerr << "[MovementFSMDatabase] Duplicate SkillID: " << id << std::endl;
				return;
			}
			definitions[id] = def;
			std::cout << "[SkillDatabase.h(AddSkill): Skill id ] added" << id << "\"" << std::endl;
		}

		bool Has(const std::string& id) const
		{
			return definitions.find(id) != definitions.end();
		}

		const MovementFSMDefinition& Get(const std::string& id) const
		{
			return definitions.at(id);
		}


	};
}