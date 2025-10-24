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
		float windowSec = 6.0f;		// 初回 CC 被弾時 から 6.0sec の間 を AntiChain単位 として 設定
		float immunitySec = 5.0f;	// CC 無効時間は Window内で Count>=2 になってそのCCが解除された瞬間から 4.0sec
		// デフォルトでApplyImmune
		// AntiChainAction action = AntiChainAction::DenyRequests;

		std::unordered_map<std::type_index, int> weightByCC;


		// CC再適用ポリシ => デフォルトでReplace
		//enum class ReapplyPolicy { Ignore, RefreshTime, Replace, Queue };
		//ReapplyPolicy reapply = ReapplyPolicy::Ignore;

		// ヘルパ 重みづけ取得
		int weightOf(std::type_index cc) const {
			if (auto it = weightByCC.find(cc); it != weightByCC.end()) return it->second;
			return 1;
		}

		static CCAntiChainPolicy Default() { return CCAntiChainPolicy{}; }

	};
}