#pragma once

#include "Game/Character/Private/FSM/Movement/Effect/StateScoped/Template/IEffectTemplate.hpp"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"
#include "Game/Character/Stats/Public/StatsComponent.hpp"

#include <glm/glm.hpp>

// これは不使用なので削除可
namespace Game::Character::FSM::Movement::Effect
{

	// IDLE -> MOVING
	struct ApplyMovementVelocity : IEffectTemplate
	{
		glm::vec2 direction;

		void apply(Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity entity, const MovementFSMContext& ctx) const override
		{
			if (!ecs.hasComponent<Engine::ECS::Component::Logic2D::Velocity2DComponent>(entity)) return ;
			if(!ecs.hasComponent<Game::Character::Stats::CharacterStatsComponent>(entity)) return;

			auto& vel = ecs.get<Engine::ECS::Component::Logic2D::Velocity2DComponent>(entity);
			const auto& stats = ecs.get<Game::Character::Stats::CharacterStatsComponent>(entity);

			vel.velocity = glm::normalize(ctx.direction) * stats.moveSpeed;
		}
	};

	// MOVING -> IDLE
	struct ResetMovementVelocity : IEffectTemplate
	{
		void apply(Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity entity, const MovementFSMContext& ctx) const override
		{
			if (!ecs.hasComponent<Engine::ECS::Component::Logic2D::Velocity2DComponent>(entity)) return;

			auto& vel = ecs.get<Engine::ECS::Component::Logic2D::Velocity2DComponent>(entity);

			vel.velocity = glm::vec2(0.0f);

			// std::cout << "[FSMResolverSystem] ctx.direction = ("
			// 	<< ctx.direction.x << ", " << ctx.direction.y << ")\n";

		}
	};
}