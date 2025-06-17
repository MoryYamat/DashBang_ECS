#include "InitStateTransitionDatabase.hpp"

#include "Game/Character/State/Behavior/CharacterState.h"
#include "Game/Character/State/Rule/StateTransitionRule.hpp"

#include "Game/Character/State/Rule/StateTransitionDatabase.hpp"

#include <iostream>

void Game::Character::State::InitCharacterStateTransitionDatabase(eNsECS::EntityMgr& ecs)
{
	auto& db = ecs.createResource<StateTransitionDatabase>();

	using State = CharacterBehaviorState;
	using ConditionType = TransitionConditionType;

	db.rules =
	{
		// Idle → Moving：CCされていないとき移動開始
		{
			.from = State::Idle,
			.to = State::Moving,
			.conditions = {
				{.type = ConditionType::NotCrowdControlled}
			}
		},

		// Moving → Idle：何もしていないとき移動終了
		{
			.from = State::Moving,
			.to = State::Idle,
			.conditions = {}
		},

		// Idle → Action：行動開始（CCなし＆中断可能）
		{
			.from = State::Idle,
			.to = State::Action,
			.conditions = {
				{.type = ConditionType::NotCrowdControlled },
				{.type = ConditionType::CanBeInterrupted }
			}
		},

		// Action → Idle：行動終了（一定時間経過後）
		{
			.from = State::Action,
			.to = State::Idle,
			.conditions = {
				{ .type = ConditionType::HasTimeElapsed, .floatValue = 1.5f }
			}
		},
	};

	std::cout << "[InitStateTransitionDatabase.cpp(init)]: Character state transition conditions initialization completed successfully.\n";
}