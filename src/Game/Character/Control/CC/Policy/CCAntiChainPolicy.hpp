#pragma once

#include <typeindex>
#include <unordered_map>

namespace Game::Character::Control::CC::Policy
{
	//enum class AntiChainAction
	//{
	//	ApplyImmune,// 閾値到達->IMMUNEへ遷移リクエストを投げる
	//	DenyRequests,// 閾値到達->一定時間 None->CCリクエストを入り口で拒否
	//};

	struct CCAntiChainPolicy
	{
		int threshold = 2;
		float windowSec = 6.0f;
		float immunitySec = 5.0f;
		// デフォルトでApplyImmune
		// AntiChainAction action = AntiChainAction::DenyRequests;

		std::unordered_map<std::type_index, int> weightByCC;


		// CC再適用ポリシ => デフォルトでReplace
		//enum class ReapplyPolicy { Ignore, RefreshTime, Replace, Queue };
		//ReapplyPolicy reapply = ReapplyPolicy::Ignore;

		// ヘルパ
		int weightOf(std::type_index cc) const {
			if (auto it = weightByCC.find(cc); it != weightByCC.end()) return it->second;
			return 1;
		}

		static CCAntiChainPolicy Default() { return CCAntiChainPolicy{}; }

	};
}