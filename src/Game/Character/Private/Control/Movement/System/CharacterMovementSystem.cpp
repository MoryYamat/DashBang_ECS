#include "CharacterMovementSystem.h"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Game/Character/Private/Stats/Component/CharacterStatsComponent.h"

#include "Game/Character/Private/Control/Movement/Component/Intent/MovementIntentComponent.h"
#include "Game/Character/Private/Control/Movement/Component/Intent/FacingIntentComponent.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

// 使用中
void Game::Character::Movement::CharacterMovementSystem(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	namespace Ops = Engine::ECS::Ops;
	namespace Comp = Engine::ECS::Component;

	for (Engine::ECS::Entity e : ecs.view<
		Comp::Logic2D::Logic2DTransformComponent
		, Comp::Logic2D::Velocity2DComponent>())
	{
		auto& vel = Ops::Get<Comp::Logic2D::Velocity2DComponent>(ecs, e);
		auto& logic = Ops::Get<Comp::Logic2D::Logic2DTransformComponent>(ecs, e);

		// 位置を速度に基づいて更新
		logic.positionXZ += vel.velocity * deltaTime;

	}
}