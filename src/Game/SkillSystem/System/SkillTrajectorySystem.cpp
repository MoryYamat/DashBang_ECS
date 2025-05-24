#include "SkillTrajectorySystem.h"

#include "Game/SkillSystem/Factory/SkillTrajectoryFactory.h"

void SkillTrajectorySystem::Update(ECS& ecs, float deltaTime)
{
	for(Entity e : ecs.view<SkillTrajectoryComponent, Transform2DComponent>())
	{
		auto& trajComp = ecs.get<SkillTrajectoryComponent>(e);
		auto& transform = ecs.get<Transform2DComponent>(e);

		trajComp.elapsedTime += deltaTime;


		transform = trajComp.trajectoryFunc(trajComp.elapsedTime);
	}
}