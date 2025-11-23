#include "BuildLocomotionAnimationQuerySystem.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"



#include "Engine/ECS/Component/Graphics/AnimatorComponent.hpp"
#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Game/Character/Private/FSM/Movement/StateModel/MovementStateComponent.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace Game::Character::Animation::Query
{
	constexpr float kEps = 1e-6f;


	namespace Ops = Engine::ECS::Ops;

	namespace Grf = Engine::ECS::Component::Graphics;
	namespace Logic2D = Engine::ECS::Component::Logic2D;

	namespace Mv = Game::Character::FSM::Movement;

	void BuildLocomotionAnimationQuerySystem(Engine::ECS::EntityMgr& ecs)
	{

		//for (auto e : ecs.view<
		//	Mv::MovementStateComponent,
		//	Logic2D::Logic2DTransformComponent,
		//	Logic2D::Velocity2DComponent,
		//	LocomotionAnimQueryComponent
		//>())
		//{
		//	auto& query = Ops::Get<LocomotionAnimQueryComponent>(ecs, e);
		//	const auto& state = Ops::Get<Mv::MovementStateComponent>(ecs, e);
		//	
		//	const auto& transform = Ops::Get<Logic2D::Logic2DTransformComponent>(ecs, e);
		//	const auto& vel = Ops::Get<Logic2D::Velocity2DComponent>(ecs, e);
		//	

		//	const glm::vec2 facing = transform.front;
		//	const bool isMoving = (glm::length2(vel.velocity) > kEps);
		//	const glm::vec2 moveDir = isMoving ? glm::normalize(vel.velocity) : facing;

		//	query.isMoving = isMoving;
		//	query.facingDirWorld = facing;
		//	query.moveDirWorld = moveDir;
		//	// query.moveSpeed = glm::length(vel.velocity);
		//	//query.normalizedSpeed = 

		//	query.valid = true;

		//	// std::cout << "[ ] " << query.valid << "\n";

		//}
	
	
	}
}