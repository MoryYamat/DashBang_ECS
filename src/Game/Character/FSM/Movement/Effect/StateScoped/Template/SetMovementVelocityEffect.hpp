#pragma once

#include "Game/Character/FSM/Movement/Effect/StateScoped/Template/IEffectTemplate.hpp"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"
#include "Game/Character/Stats/Component/CharacterStatsComponent.h"

#include <glm/glm.hpp>

// これは不使用なので削除可
namespace Game::Character::FSM::Movement::Effect
{
	using namespace Engine::ECS::Component::Logic2D;
	using namespace Game::Character::Stats;

	// IDLE -> MOVING
	struct ApplyMovementVelocity : IEffectTemplate
	{
		glm::vec2 direction;

		void apply(eNsECS::EntityMgr& ecs, eNsECS::Entity entity, const MovementFSMContext& ctx) const override
		{
			if (!ecs.hasComponent<Velocity2DComponent>(entity)) return ;
			if(!ecs.hasComponent<CharacterStatsComponent>(entity)) return;

			auto& vel = ecs.get<Velocity2DComponent>(entity);
			const auto& stats = ecs.get<CharacterStatsComponent>(entity);

			vel.velocity = glm::normalize(ctx.direction) * stats.moveSpeed;
		}
	};

	// MOVING -> IDLE
	struct ResetMovementVelocity : IEffectTemplate
	{
		void apply(eNsECS::EntityMgr& ecs, eNsECS::Entity entity, const MovementFSMContext& ctx) const override
		{
			if (!ecs.hasComponent<Velocity2DComponent>(entity)) return;

			auto& vel = ecs.get<Velocity2DComponent>(entity);

			vel.velocity = glm::vec2(0.0f);

			// std::cout << "[FSMResolverSystem] ctx.direction = ("
			// 	<< ctx.direction.x << ", " << ctx.direction.y << ")\n";

		}
	};
}