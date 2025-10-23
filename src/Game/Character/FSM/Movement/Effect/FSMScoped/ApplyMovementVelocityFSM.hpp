#pragma once

#include "Engine/Time/Private/WorldClock.hpp"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Game/Character/Stats/Component/CharacterStatsComponent.h"

#include "Game/Character/FSM/Movement/StateModel/MovementStateComponent.hpp"

#include "Game/Character/FSM/Movement/Effect/FSMScoped/IFSMScopedEffect.hpp"

#include "Game/Character/FSM/Movement/MovementStateTags.hpp"

// modifier
#include "Game/Combat/Skill/FSM/Modifier/Movement/MovementSpeedModifier.hpp"

#include "Game/Common/Logic/FSM/Query/FSMQuery.hpp"

#include <iostream>

#include <glm/glm.hpp>

namespace Game::Character::FSM::Movement::Effect::FSMScoped
{


	struct ApplyMovementVelocityFSM : IFSMScopedEffect
	{

		void update(Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity entity
			, const Game::Character::FSM::Movement::MovementFSMContext& ctx, float deltaTime) const override
		{
			using namespace Engine::ECS::Component::Logic2D;
			using namespace Game::Character::Stats;
			using namespace Game::Character::FSM::Movement;
			using namespace Game::Common::Logic::FSM::Query;
			using namespace Game::Combat::Skill::FSM;

			const auto& clock = Engine::Time::worldClock(ecs);

			if (!ecs.hasComponent<Velocity2DComponent>(entity)) return;
			if (!ecs.hasComponent<CharacterStatsComponent>(entity)) return;
			if (!ecs.hasComponent<MovementStateComponent>(entity)) return;

			
			auto& vel = ecs.get<Velocity2DComponent>(entity);
			const auto& stats = ecs.get<CharacterStatsComponent>(entity);
			const auto& state = ecs.get<MovementStateComponent>(entity);

			// std::cout << "[ApplyMovementVelocityFSM] ctx.direction = ("
			// 	<< ctx.direction.x << ", " << ctx.direction.y << ") / clock = "
			// 	<< clock.now << "\n";


			// この分岐がここで必要かは要検討
			if (state.current == StateTag::MOVING)
			{
				const float multiplier = Game::Combat::Skill::FSM::Modifier::Movement::CalcMovementSpeedMultiplierFromSkillFSM(ecs, entity);
				vel.velocity = glm::normalize(ctx.direction) * stats.moveSpeed * multiplier;

			}
			else
				vel.velocity = glm::vec2(0.0f);// 
		}
	};
}