#include "UpdateCharacterVelocity.h"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Game/Character/Movement/Component/Intent/MovementIntentComponent.h"

#include "Game/Character/Stats/Component/CharacterStatsComponent.h"

#include "Common/GameNamespaceDecl.h"

#include <GLM/glm.hpp>

void Game::Character::Movement::UpdateCharacterVelocity(eNsECS::EntityMgr& ecs)
{
	for (eNsECS::Entity e : ecs.view<
		eNsLogic2DComp::Velocity2DComponent
		, gNsCharacterIntent::MovementIntentComponent
		, gNsCharacterStats::CharacterStatsComponent
	>())
	{
		auto& vel = ecs.get< eNsLogic2DComp::Velocity2DComponent>(e);
		auto& intent = ecs.get<gNsCharacterIntent::MovementIntentComponent>(e);
		auto& stats = ecs.get<gNsCharacterStats::CharacterStatsComponent>(e);

		if (!intent.isActive || glm::length(intent.direction) < 0.001f)
		{
			vel.velocity = glm::vec2(0.0f);
			continue;
		}

		vel.velocity = glm::normalize(intent.direction) * stats.moveSpeed;
	}
}