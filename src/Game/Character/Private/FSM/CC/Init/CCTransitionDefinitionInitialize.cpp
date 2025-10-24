#include "CCTransitionDefinitionInitialize.hpp"

#include "Game/Character/Private/FSM/CC/Database/CCFSMDatabase.hpp"

#include "Game/Character/Private/FSM/CC/StateModel/CCFSMDefinition.hpp"
#include "Game/Character/Private/FSM/CC/StateModel/CCTransitionTable.hpp"

#include "Game/Character/Private/FSM/CC/StateModel/CCFSMConditions.hpp"

#include "Game/Character/Private/FSM/CC/CCStateTags.hpp"

// effect
#include "Game/Character/Private/FSM/CC/Effect/StateScoped/Hook/CCFSMStateEffectHook.hpp"
#include "Game/Character/Private/FSM/CC/Effect/StateScoped/TriggerCondition/IStateEffectTriggerCondition.hpp"
#include "Game/Character/Private/FSM/CC/Effect/StateScoped/Handler/Interference/emitMovementInterference.hpp"
#include "Game/Character/Private/FSM/CC/Effect/StateScoped/Handler/Interference/emitSkillInterference.hpp"

// reset
#include "Game/Character/Private/FSM/CC/Reset/ResetHookDefinition.hpp"
#include "Game/Character/Private/FSM/CC/Reset/Handler/ClearEffectExecutionLog.hpp"
#include "Game/Character/Private/FSM/CC/Reset/Trigger/OnResetTransition.hpp"

#include <optional>
#include <string>
#include <memory>

void Game::Character::FSM::CC::InitCCTransitionDefinitionDatabase(Engine::ECS::EntityMgr& ecs)
{
	using namespace Game::Character::FSM::CC::Database;
	using namespace Game::Character::FSM::CC::StateModel;
	using namespace Game::Character::FSM::CC::StateEffect;
	using namespace Game::Character::FSM::CC::Reset;

	auto& db = ecs.createResource<CCFSMDatabase>();

	float stunSec = 5.0f;
	float kdSec = 5.0f;

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

	// stun
	def.hooks.push_back(
		{
			.handler = std::make_shared<emitMovementInterference>(stunSec),
			.trigger = std::make_shared<OnTransition>(std::nullopt, StateTag::STUNNED)
		}
	);
	def.hooks.push_back(
		{
			.handler = std::make_shared<emitSkillInterference>(stunSec),
			.trigger = std::make_shared<OnTransition>(std::nullopt, StateTag::STUNNED)
		}
	);

	// knockdown
	def.hooks.push_back(
		{
			.handler = std::make_shared<emitMovementInterference>(kdSec),
			.trigger = std::make_shared<OnTransition>(std::nullopt, StateTag::KNOCKDOWNED)
		});
	def.hooks.push_back(
		{
			.handler = std::make_shared<emitSkillInterference>(kdSec),
			.trigger = std::make_shared<OnTransition>(std::nullopt, StateTag::KNOCKDOWNED)
		});

	// reset
	def.resetHooks = 
	{
		CCFSMResetHook
		{
			.handlers =
			{
				std::make_shared<ClearEffectExecutionLog>(),
			},
			.trigger = std::make_shared<OnResetTransition>(std::nullopt, StateTag::NONE),
		},
	};


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