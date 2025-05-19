// 全スキルの「静的データ」を管理
// Manage "static data" for all skills

#pragma once

#include <unordered_map>
#include "Game/SkillSystem/MasterData/SkillDefinition.h"

#include <iostream>

class SkillDatabase
{
public:
	void AddSkill(const SkillDefinition& def)
	{
		if (mDefinitions.count(def.id))
		{
			std::cerr << "[SkillDatabase] Duplicate SkillID: " << def.id << std::endl;
			return;
		}
		mDefinitions[def.id] = def;
		std::cout << "[SkillDatabase.h(AddSkill): Skill id ]" << def.id << "is added as \"" << def.name << "\"" << std::endl;
	}

	const SkillDefinition& Get(int id) const
	{
		auto it = mDefinitions.find(id);
		if (it == mDefinitions.end())
		{
			throw std::out_of_range("[SkillDatabase::Get] SkillID not found: " + std::to_string(id));
		}
		return it->second;
	}

private:

	std::unordered_map<int, SkillDefinition> mDefinitions;
};