#include "CharacterStateSystem.h"

#include "Game/Character/State/Component/CharacterStateComponent.h"

#include "Game/Character/State/Rule/StateTransitionDatabase.hpp"

#include "Game/Character/State/Rule/StateTransitionEvaluator.hpp"

#include "Common/GameNamespaceDecl.h"

#include <iostream>

void Game::Character::State::CharacterStateSystem::UpdateStates(eNsECS::EntityMgr& ecs, float deltaTime)
{
	auto& ruleDatabase = ecs.getResource<gNsCharacterState::StateTransitionDatabase>();

	// std::cout << "view ÇÃ typeid: " << typeid(decltype(ecs.view<gNsCharacterState::CharacterStateComponent>())).name() << std::endl;

	for (eNsECS::Entity e : ecs.view<gNsCharacterState::CharacterStateComponent>())
	{

		auto& state = ecs.get<gNsCharacterState::CharacterStateComponent>(e);

		state.timeInState += deltaTime;

		//std::cout << "[CharacterStateSystem.cpp(NowState)] Entity " << e.id
		//	<< ": " << ToString(state.current) << std::endl;

		if (!state.requestedNextState.has_value())
			continue;

		gNsCharacterState::CharacterBehaviorState from = state.current;
		gNsCharacterState::CharacterBehaviorState to = *state.requestedNextState;

		
		// ëJà⁄ÉãÅ[ÉãéÊìæ
		const auto* rule = ruleDatabase.FindRule(from, to);
		if (rule && gNsCharacterState::StateTransitionEvaluator::AreConditionsMet(state, deltaTime, rule->conditions))
		{
			state.previous = state.current;
			state.current = to;
			state.timeInState = 0.0f;

			std::cout << "[CharacterStateSystem.cpp(StateChange)] Entity " << e.id
				<< ": " << ToString(state.previous)
				<< " Å® " << ToString(state.current) << std::endl;

		}

		state.requestedNextState.reset();




	}

	// std::cout << "[CharacterStateSystem.cpp(Update)]: test deltaTime: " << deltaTime << "\n";
}

void Game::Character::State::CharacterStateSystem::RequestStateChange(
	eNsECS::EntityMgr& ecs
	, eNsECS::Entity e
	, CharacterBehaviorState nextState
)
{
	auto& state = ecs.get<CharacterStateComponent>(e);

	if (state.current == CharacterBehaviorState::Dead)
	{
		gNsCharacterState::ToString(state.current);
		return;
	}

	state.requestedNextState = nextState;
}
void Game::Character::State::CharacterStateSystem::ForceCrowdControl(
	eNsECS::EntityMgr& ecs
	, eNsECS::Entity e
	, CharacterCrowdControlState ccState
)
{
	auto& state = ecs.get<gNsCharacterState::CharacterStateComponent>(e);
	state.crowdControl = ccState;

	// é©ìÆìIÇ…çsìÆÇé~ÇﬂÇÈ
	if (ccState != gNsCharacterState::CharacterCrowdControlState::None)
	{
		state.requestedNextState = gNsCharacterState::CharacterBehaviorState::Idle;
		state.canBeInterrupted = true;
	}
}