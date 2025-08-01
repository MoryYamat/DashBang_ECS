#include "CharacterMovementIntentResolver.hpp"

#include "Game/Character/Control/Movement/Component/Intent/MovementIntentComponent.h"
#include "Game/Character/FSM/Movement/StateModel/MovementFSMTransitionRequestComponent.hpp"

#include "Game/Character/FSM/Movement/StateModel/MovementStateComponent.hpp"

#include "Game/Character/FSM/Movement/MovementStateTags.hpp"

#include <typeindex>
#include <cstdint>
#include <glm/glm.hpp>

// FIXME: ‚»‚à‚»‚à•s—v‚Ì‰Â”\«
// TDDO: FSM\‘¢‚ÌŒ^(í’“Œ^ / ƒgƒŠƒK[Œ^)‚ÌØ‚è•ª‚¯
// TODO: .priority‚Ì§Œä
void Game::Character::Control::Movement::UpdateMovementIntentResolverSystem(eNsECS::EntityMgr& ecs)
{
	using namespace Game::Character::FSM::Movement;
	using namespace Game::Character::Intent;

	for (eNsECS::Entity e : ecs.view<
		MovementIntentComponent,
		MovementStateComponent,
		MovementFSMTransitionRequestComponent>())
	{
		const auto& intent = ecs.get<MovementIntentComponent>(e);
		const auto& state = ecs.get<MovementStateComponent>(e);
		auto& reqComp = ecs.get<MovementFSMTransitionRequestComponent>(e);

		std::type_index requested =
			(intent.isActive && glm::length(intent.direction) > 0.001f)
			? StateTag::MOVING
			: StateTag::IDLE;

		if (state.current == requested) continue;

		// FIXME: state, **priority** 
		constexpr uint32_t defaultPriority = 0;
		if (reqComp.hasExactRequest(requested, defaultPriority)) continue;

		reqComp.requests.push_back(
			{
				MovementFSMTransitionRequest{
				.requestedTo = requested,
				.priority = defaultPriority
			} });

	}
}