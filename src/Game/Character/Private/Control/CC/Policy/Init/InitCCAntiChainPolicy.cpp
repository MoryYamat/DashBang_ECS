#include "InitCCAntiChainPolicy.hpp"

#include "Game/Character/Private/Control/CC/Policy/CCAntiChainPolicy.hpp"
#include "Game/Character/Private/Control/CC/Policy/CCAntiChainPolicyDatabase.hpp"

#include "Game/Character/Private/FSM/CC/CCStateTags.hpp"

namespace Game::Character::Control::CC::Policy
{
	using namespace Game::Character::FSM::CC;

	// TODO: IDの自動一致機能の作成
	void InitCCAntiChainPolicy(Engine::ECS::EntityMgr& ecs)
	{
		auto& db = ecs.createResource<CCAntiChainPolicyDatabase>();
		
		CCAntiChainPolicy def = CCAntiChainPolicy::Default();

		// 必要があればPolicyの変更
		def.weightByCC[StateTag::STUNNED] = 1;
		def.weightByCC[StateTag::KNOCKDOWNED] = 1;


		db.Add("default", def);

		// 既存のFSM定義IDに対して紐づけ
		db.BindFSM("basic", "default");
	}

}