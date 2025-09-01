#include "CCTransitionDefinitionInitialize.hpp"

#include "Game/Character/FSM/CC/Database/CCFSMDatabase.hpp"

#include "Game/Character/FSM/CC/StateModel/CCFSMDefinition.hpp"
#include "Game/Character/FSM/CC/StateModel/CCTransitionTable.hpp"

#include "Game/Character/FSM/CC/StateModel/CCFSMConditions.hpp"

#include "Game/Character/FSM/CC/CCStateTags.hpp"

#include <string>
#include <memory>

void Game::Character::FSM::CC::InitCCTransitionDefinitionDatabase(eNsECS::EntityMgr& ecs)
{
	using namespace Game::Character::FSM::CC::Database;
	using namespace Game::Character::FSM::CC::StateModel;

	auto& db = ecs.createResource<CCFSMDatabase>();

	float stunSec = 2.0f;
	float kdSec = 2.0f;

	CCFSMDefinition def;
	def.initialState = StateTag::NONE;

	// HitEvent -> 解析 -> リクエストによる方式に変更
	//def.transitions.push_back(
	//	{
	//		.from = StateTag::NONE,
	//		.to = StateTag::STUNNED,
	//		.condition = std::make_shared<AlwaysTrue>(),
	//		.priority = 100// これはCCFSM自体の遷移定義なので別に0でもよい
	//	}
	//);

	//def.transitions.push_back(
	//	{
	//		.from = StateTag::NONE,
	//		.to = StateTag::KNOCKDOWNED,
	//		.condition = std::make_shared <AlwaysTrue>(),
	//		.priority = 100// これはCCFSM自体の遷移定義なので別に0でもよい
	//	}
	//);

	def.transitions.push_back(
		{
			.from = StateTag::STUNNED,
			.to = StateTag::NONE,
			.condition = std::make_shared<CCElapsedAtLeast>(stunSec),
			.priority = 100
		}
	);

	def.transitions.push_back(
		{
			.from = StateTag::KNOCKDOWNED,
			.to = StateTag::NONE,
			.condition = std::make_shared<CCElapsedAtLeast>(kdSec),
			.priority = 100
		}
	);

	// TODO:
	// FSMSystem
	// FSMResolver
	// priorityの設計・実装
	// CCからの復帰定義設計・実装
	// 干渉の設計・実装
	// 
	//
	
	db.Add("basic", def);
}