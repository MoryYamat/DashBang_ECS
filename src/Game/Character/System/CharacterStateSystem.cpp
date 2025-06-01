#include "CharacterStateSystem.h"

#include "Game/Character/Component/CharacterStateComponent.h"

#include "Common/GameNamespaceDecl.h"

void Game::Character::State::CharacterStateSystem::UpdateStates(eNsECS::EntityMgr& ecs, float deltaTime)
{
	for (eNsECS::Entity e : ecs.view<CharacterStateComponent>())
	{
		auto& state = ecs.get<CharacterStateComponent>(e);

		state.timeInState += deltaTime;

		if (state.requestedNextState.has_value())
		{
			if (state.current == CharacterBehaviorState::Action && !state.canBeInterrupted)
				continue;

			state.previous = state.current;
			state.current = *state.requestedNextState;
			state.timeInState = 0.0f;
			state.requestedNextState.reset();
			gNsCharacterState::ToString(state.current);
		}
	}
}

void Game::Character::State::CharacterStateSystem::RequestStateChange(eNsECS::EntityMgr& ecs, eNsECS::Entity e, CharacterBehaviorState nextState)
{
	auto& state = ecs.get<CharacterStateComponent>(e);

	if (state.current == CharacterBehaviorState::Dead)
	{
		gNsCharacterState::ToString(state.current);
		return;
	}

	state.requestedNextState = nextState;
}
void Game::Character::State::CharacterStateSystem::ForceCrowdControl(eNsECS::EntityMgr& ecs, eNsECS::Entity e, CharacterCrowdControlState ccState)
{

}