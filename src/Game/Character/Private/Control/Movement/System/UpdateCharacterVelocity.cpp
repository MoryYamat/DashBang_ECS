#include "UpdateCharacterVelocity.h"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Game/Character/Private/Control/Movement/Component/Intent/MovementIntentComponent.h"

#include "Game/Character/Private/Stats/Public/StatsComponent.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <GLM/glm.hpp>

// 削除予定：FSMResolver導入後廃止
void Game::Character::Movement::UpdateCharacterVelocity(Engine::ECS::EntityMgr& ecs)
{
	namespace Ops = Engine::ECS::Ops;
	namespace Comp = Engine::ECS::Component;

	for (Engine::ECS::Entity e : ecs.view<
		Comp::Logic2D::Velocity2DComponent,
		Game::Character::Intent::MovementIntentComponent,
		Game::Character::Stats::CharacterStatsComponent
	>())
	{
		auto& vel = Ops::Get<Comp::Logic2D::Velocity2DComponent>(ecs, e);
		auto& intent = Ops::Get<Game::Character::Intent::MovementIntentComponent>(ecs, e);
		auto& stats = Ops::Get<Game::Character::Stats::CharacterStatsComponent>(ecs, e);

		if (!intent.isActive || glm::length(intent.direction) < 0.001f)
		{
			vel.velocity = glm::vec2(0.0f);
			continue;
		}

		vel.velocity = glm::normalize(intent.direction) * stats.moveSpeed;
	}
}