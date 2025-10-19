#pragma once

#include "Game/Character/FSM/CC/StateModel/CCFSMDefinition.hpp"

#include <string>
#include <unordered_map>
#include <iostream>
namespace Game::Character::FSM::CC::Database
{

	struct CCFSMDatabase
	{
		std::unordered_map<std::string, Game::Character::FSM::CC::StateModel::CCFSMDefinition> definitions;

		void Add(const std::string& id, const Game::Character::FSM::CC::StateModel::CCFSMDefinition& def)
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

		const Game::Character::FSM::CC::StateModel::CCFSMDefinition& Get(const std::string& id) const
		{
			return definitions.at(id);
		}
	};
}