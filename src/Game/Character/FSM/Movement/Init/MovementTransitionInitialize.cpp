#include "MovementTransitionInitialize.hpp"

#include "Game/Character/FSM/Movement/Database/MovementFSMDatabase.hpp"

#include "Game/Character/FSM/Movement/StateModel/MovementFSMDefinition.hpp"

#include "Game/Character/FSM/Movement/Effect/FSMScoped/ApplyMovementVelocityFSM.hpp"
#include "Game/Character/FSM/Movement/Effect/FSMScoped/IFSMScopedEffect.hpp"

#include "Game/Character/FSM/Movement/Effect/StateScoped/Template/SetMovementVelocityEffect.hpp"
#include "Game/Character/FSM/Movement/Effect/StateScoped/TriggerCondition/WhileInState.hpp"

#include "Game/Character/FSM/Movement/StateModel/MovementTransitionTable.hpp"

#include "Game/Character/FSM/Movement/MovementStateTags.hpp"

#include "Common/GameNamespaceDecl.h"

#include <memory>

void Game::Character::FSM::Movement::InitMovementFSMDatabase(eNsECS::EntityMgr& ecs)
{
	using namespace Game::Character::FSM::Movement;
	using namespace Game::Character::FSM::Movement::StateTag;
	using namespace Game::Character::FSM::Movement::Database;
	using namespace Game::Character::FSM::Movement::Effect;
	using namespace Game::Character::FSM::Movement::Effect::FSMScoped;


	auto& db = ecs.createResource<MovementFSMDatabase>();

	MovementFSMDefinition def;
	def.initialState = IDLE;

	def.transitions.push_back(
		{
			.from = IDLE,
			.to = MOVING,
			.condition = std::make_shared<IsMoveIntentActive>()
		});

	def.transitions.push_back(
		{
			.from = MOVING,
			.to = IDLE,
			.condition = std::make_shared<IsStopIntent>()
		});

	def.fsmScopedEffects.push_back(
		std::make_shared<ApplyMovementVelocityFSM>()
	);

	db.Add("basic", def);
}

//void Game::Character::FSM::Movement::InitializeMovementTransitionSystem(eNsECS::EntityMgr& ecs)
//{
//	auto& tableRes = ecs.createResource<gNsCharaFSMMovement::MovementTransitionTableResource>();
//
//	tableRes.transitions = gNsCharaFSMMovement::BuildMovementTransitionTable();
//}