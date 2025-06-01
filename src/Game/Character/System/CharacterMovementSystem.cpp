#include "CharacterMovementSystem.h"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Game/Character/Component/CharacterStatsComponent.h"

#include "Game/Character/Component/Intent/MovementIntentComponent.h"

#include "Common/GameNamespaceDecl.h"


void Game::Character::Movement::UpdateCharacterVelocity(eNsECS::EntityMgr& ecs)
{
	for (eNsECS::Entity e : ecs.view<gNsCharacterIntent::MovementIntentComponent, eNsLogic2DComp::Velocity2DComponent, CharacterStatsComponent>())
	{
		auto& intent = ecs.get<gNsCharacterIntent::MovementIntentComponent>(e);
		auto& vel = ecs.get<eNsLogic2DComp::Velocity2DComponent>(e);
		auto& stats = ecs.get<CharacterStatsComponent>(e);

		// 
		if (!intent.isActive || glm::length(intent.direction) < 0.001f)
		{
			vel.velocity = glm::vec2(0.0f);
			continue;
		}

		// 速度更新
		vel.velocity = glm::normalize(intent.direction) * stats.moveSpeed;
	}
}

//void Game::Character::Movement::UpdateMovement(ECS& ecs, float deltaTime)
//{
//	for (Entity e : ecs.view<Logic2DTransformComponent, MovementIntentComponent>())
//	{
//		auto& logic = ecs.get<Logic2DTransformComponent>(e);
//		auto& intent = ecs.get<MovementIntentComponent>(e);
//
//		if (!intent.isActive || glm::length(intent.direction) < 0.001f)
//			continue;
//
//		glm::vec2 moveDir = glm::normalize(intent.direction);
//		logic.positionXZ += moveDir * intent.speed * deltaTime;
//
//		// 向き更新は別ロジックで
//	}
//}