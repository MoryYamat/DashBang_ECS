#pragma once

#include "Game/Character/FSM/CC/StateModel/CCFSMDefinition.hpp"

#include <string>
#include <unordered_map>
#include <iostream>
namespace Game::Character::FSM::CC::Database
{
	using namespace Game::Character::FSM::CC::StateModel;

	struct CCFSMDatabase
	{
		std::unordered_map<std::string, CCFSMDefinition> definitions;

		void Add(const std::string& id, const CCFSMDefinition& def)
		{
			if (definitions.count(id))
			{
				std::cerr << "[CCFSMDatabase] Duplicate CCFSMDef: " << id << std::endl;
				return;
			}

			definitions[id] = def;
			std::cout << "[CCFSMDatabase.hpp(AddCCFSMDef): CCFSMDef id ] added" << id << "\"" << std::endl;
		}


		bool Has(const std::string& id) const
		{
			return definitions.find(id) != definitions.end();
		}

		const CCFSMDefinition& Get(const std::string& id)
		{
			return definitions.at(id);
		}
	};
}