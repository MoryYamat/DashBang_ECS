#pragma once

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Game/Character/Stats/Component/CharacterStatsComponent.h"

#include "Game/Character/FSM/Movement/StateModel/MovementStateComponent.hpp"

#include "Game/Character/FSM/Movement/Effect/FSMScoped/IFSMScopedEffect.hpp"

#include "Game/Character/FSM/Movement/MovementStateTags.hpp"

#include "Common/EngineNamespaceDecl.h"

#include <iostream>

#include <glm/glm.hpp>

namespace Game::Character::FSM::Movement::Effect::FSMScoped
{
	using namespace Engine::ECS::Component::Logic2D;
	using namespace Game::Character::Stats;
	using namespace Game::Character::FSM::Movement;

	struct ApplyMovementVelocityFSM : IFSMScopedEffect
	{
		void update(eNsECS::EntityMgr& ecs, eNsECS::Entity entity, const MovementFSMContext& ctx, float deltaTime) const override
		{
			if (!ecs.hasComponent<Velocity2DComponent>(entity)) return;
			if (!ecs.hasComponent<CharacterStatsComponent>(entity)) return;
			if (!ecs.hasComponent<MovementStateComponent>(entity)) return;

			auto& vel = ecs.get<Velocity2DComponent>(entity);
			const auto& stats = ecs.get<CharacterStatsComponent>(entity);
			const auto& state = ecs.get<MovementStateComponent>(entity);

			//std::cout << "[ApplyMovementVelocityFSM] ctx.direction = ("
			//	<< ctx.direction.x << ", " << ctx.direction.y << ")\n";


			// ‚±‚Ì•ªŠò‚ª‚±‚±‚Å•K—v‚©‚Í—vŒŸ“¢
			if (state.current == StateTag::MOVING)
			{
				std::cout << "here\n";
				vel.velocity = glm::normalize(ctx.direction) * stats.moveSpeed;

			}
			else
				vel.velocity = glm::vec2(0.0f);// 
		}
	};
}