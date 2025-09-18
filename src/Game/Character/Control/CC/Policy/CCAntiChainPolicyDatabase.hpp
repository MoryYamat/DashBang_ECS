#pragma once

#include "Game/Character/Control/CC/Policy/CCAntiChainPolicy.hpp"

#include <string>
#include <unordered_map>

namespace Game::Character::Control::CC::Policy
{
	struct CCAntiChainPolicyDatabase
	{
		std::unordered_map<std::string, CCAntiChainPolicy> table;		// policyId -> policy
		std::unordered_map<std::string, std::string> fsmIdToPolicyId;	// fsmId -> policyId


		const CCAntiChainPolicy& Get(const std::string& id) const { return table.at(id); }
		void Add(const std::string& id, const CCAntiChainPolicy& p) { table[id] = p; }

		// FSMとの紐づけ
		void BindFSM(const std::string& fsmId, const std::string& policyId)
		{
			fsmIdToPolicyId[fsmId] = policyId;
		}

		// CCPolicyを仕様するCCFSMに基づいて検索／取得
		const CCAntiChainPolicy& ResolveForFSM(const std::string& fsmId) const
		{
			// if(init-statement ; condition)
			if (auto it = fsmIdToPolicyId.find(fsmId); it != fsmIdToPolicyId.end())
			{
				return Get(it->second);
			}
			return Get("default");
		}
	};
}