// 全スキルの「静的データ」を管理
// Manage "static data" for all skills

#pragma once

#include <unordered_map>
#include "Game/Combat/Skill/MasterData/SkillDefinition.h"

#include "Game/Combat/Skill/MasterData/SkillEntry.hpp"

#include "Common/GameNamespaceDecl.h"

#include <cstdint>

#include <iostream>

#include <cassert>

namespace Game::Combat::Skill::Database
{
	// 新バージョン：これを使えるようリファクタしていく
	class SkillDatabase
	{
	public:
		void AddSkill(const gNsSkillData::SkillEntry& entry)
		{
			if (mEntries.count(entry.def.id))
			{
				std::cerr << "[SkillDatabase] Duplicate SkillID: " << entry.def.id << std::endl;
				return;
			}
			mEntries[entry.def.id] = entry;// add key(def.id) and value(def)
			std::cout << "[SkillDatabase.h(AddSkill): Skill id ]" << entry.def.id << "is added as \"" << entry.def.name << "\"" << std::endl;
		}

		const gNsSkillData::SkillEntry& Get(const uint32_t id) const
		{
			// ~~.conatins(id)に変更予定(C++20)~~ => (.containsは存在確認をしたいだけの時.)
			//auto it = mDefinitions.find(id);
			//if (it == mDefinitions.end())
			//{
			//	throw std::out_of_range("[SkillDatabase::Get] SkillID not found: " + std::to_string(id));
			//}
			//return it->second;

			// 例外を投げる構造はよくない => そもそも存在しないIDをGetしないような構造が必要 (呼び出し側でHasなどでifする？)
			try {
				return mEntries.at(id);
			}
			catch (const std::out_of_range&) {
				throw std::out_of_range("[SkillDatabase::Get] SkillID not found: " + std::to_string(id));
			}
		}

		// 存在確認 => 存在確認(Has)と取得(Get)を一対の処理にした，tryGetを作ればいい？
		bool Has(const uint32_t id) const
		{
			return mEntries.contains(id);
		}


		// read only
		const gNsSkillData::SkillEntry* tryGet(uint32_t id) const noexcept
		{
			auto it = mEntries.find(id);
			if (it != mEntries.end())
			{
				return &it->second;
			}

			assert(it != mEntries.end());
			return nullptr;
		}

		// writable
		gNsSkillData::SkillEntry* tryGet(uint32_t id) noexcept
		{
			auto it = mEntries.find(id);
			if (it != mEntries.end())
			{
				return &it->second;
			}

			// assert(it != mEntries.end());
			return nullptr;
		}

	private:

		// ( id, skillDefinition(class) )
		std::unordered_map<uint32_t, gNsSkillData::SkillEntry> mEntries;
	};


	// read only
	[[nodiscard]]inline const SkillDatabase& skillDatabase(const Engine::ECS::EntityMgr& ecs)
	{
		return ecs.getResource<SkillDatabase>();
	}

	// writable
	[[nodiscard]] inline SkillDatabase& skillDatabase(Engine::ECS::EntityMgr& ecs)
	{
		return ecs.getResource<SkillDatabase>();
	}
}

namespace Game::Combat::Skill::Data
{



	// 古いバージョン：廃止予定
	class SkillDatabase
	{
	public:
		void AddSkill(const gNsSkillData::SkillDefinition& def)
		{
			if (mDefinitions.count(def.id))
			{
				std::cerr << "[SkillDatabase] Duplicate SkillID: " << def.id << std::endl;
				return;
			}
			mDefinitions[def.id] = def;// add key(def.id) and value(def)
			std::cout << "[SkillDatabase.h(AddSkill): Skill id ]" << def.id << "is added as \"" << def.name << "\"" << std::endl;
		}

		const gNsSkillData::SkillDefinition& Get(int id) const
		{
			// ~~.conatins(id)に変更予定(C++20)~~ => (.containsは存在確認をしたいだけの時.)
			//auto it = mDefinitions.find(id);
			//if (it == mDefinitions.end())
			//{
			//	throw std::out_of_range("[SkillDatabase::Get] SkillID not found: " + std::to_string(id));
			//}
			//return it->second;

			// 例外を投げる構造はよくない => そもそも存在しないIDをGetしないような構造が必要 (呼び出し側でHasなどでifする？)
			try {
				return mDefinitions.at(id);
			}
			catch (const std::out_of_range&) {
				throw std::out_of_range("[SkillDatabase::Get] SkillID not found: " + std::to_string(id));
			}
		}

		// 存在確認 => 存在確認(Has)と取得(Get)を一対の処理にした，tryGetを作ればいい？
		bool Has(int id) const
		{
			return mDefinitions.contains(id);
		}

	private:

		// ( id, skillDefinition(class) )
		std::unordered_map<int, gNsSkillData::SkillDefinition> mDefinitions;
	};
}

