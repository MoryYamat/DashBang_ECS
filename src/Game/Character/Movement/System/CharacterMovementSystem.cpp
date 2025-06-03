#include "CharacterMovementSystem.h"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Game/Character/Stats/Component/CharacterStatsComponent.h"

#include "Game/Character/Movement/Component/Intent/MovementIntentComponent.h"
#include "Game/Character/Movement/Component/Intent/FacingIntentComponent.h"


#include "Common/GameNamespaceDecl.h"


void Game::Character::Movement::CharacterMovementSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	for (eNsECS::Entity e : ecs.view<
		eNsLogic2DComp::Logic2DTransformComponent
		, eNsLogic2DComp::Velocity2DComponent>())
	{
		auto& vel = ecs.get<eNsLogic2DComp::Velocity2DComponent>(e);
		auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(e);

		// 位置を速度に基づいて更新
		logic.positionXZ += vel.velocity * deltaTime;

		for (eNsECS::Entity e2 : ecs.view<
			gNsCharacterIntent::FacingIntentComponent
		>())
		{
			auto& facing = ecs.get<gNsCharacterIntent::FacingIntentComponent>(e2);
			logic.front = facing.front;
		}
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
//