#include "InitCCAntiChainPolicy.hpp"

#include "Game/Character/Control/CC/Policy/CCAntiChainPolicy.hpp"
#include "Game/Character/Control/CC/Policy/CCAntiChainPolicyDatabase.hpp"

namespace Game::Character::Control::CC::Policy
{
	// TODO: IDの自動一致機能の作成
	void InitCCAntiChainPolicy(EntityMgr& ecs)
	{
		auto& db = ecs.createResource<CCAntiChainPolicyDatabase>();
		
		CCAntiChainPolicy def = CCAntiChainPolicy::Default();
		// 必要があればPolicyの変更

		db.Add("default", def);

		// 既存のFSM定義IDに対して紐づけ
		db.BindFSM("basic", "default");
	}

}