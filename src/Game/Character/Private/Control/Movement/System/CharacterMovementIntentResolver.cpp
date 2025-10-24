#include "CharacterMovementIntentResolver.hpp"

#include "Game/Character/Private/Control/Movement/Component/Intent/MovementIntentComponent.h"
#include "Game/Character/Private/FSM/Movement/StateModel/MovementFSMTransitionRequestComponent.hpp"

#include "Game/Character/Private/FSM/Movement/StateModel/MovementStateComponent.hpp"

#include "Game/Character/Private/FSM/Movement/MovementStateTags.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <typeindex>
#include <cstdint>
#include <glm/glm.hpp>

// FIXME: そもそも不要の可能性
// TDDO: FSM構造の型(常駐型 / トリガー型)の切り分け
// TODO: .priorityの制御
void Game::Character::Control::Movement::UpdateMovementIntentResolverSystem(Engine::ECS::EntityMgr& ecs)
{
	using namespace Game::Character::FSM::Movement;
	using namespace Game::Character::Intent;

	namespace Ops = Engine::ECS::Ops;

	for (Engine::ECS::Entity e : ecs.view<
		MovementIntentComponent,
		MovementStateComponent,
		MovementFSMTransitionRequestComponent>())
	{
		const auto& intent = Ops::Get<MovementIntentComponent>(ecs, e);
		const auto& state = Ops::Get<MovementStateComponent>(ecs, e);
		auto& reqComp = Ops::Get<MovementFSMTransitionRequestComponent>(ecs, e);

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