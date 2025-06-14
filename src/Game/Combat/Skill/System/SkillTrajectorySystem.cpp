#include "SkillTrajectorySystem.h"

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"
#include "Game/Combat/Skill/Factory/SkillTrajectoryFactory.h"
#include "Game/Combat/Skill/Component/SkillTrajectoryComponent.h"

#include "Engine/Debug/DebugUtils.h"

#include <iostream>

void Game::Combat::Skill::System::SkillTrajectorySystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	for(eNsECS::Entity e : ecs.view<
		gNsSkillComp::Attack2DAreaComponent,
		gNsSkillComp::SkillTrajectoryComponent,
		eNsLogic2DComp::Transform2DComponent>())
	{
		auto& trajComp = ecs.get<gNsSkillComp::SkillTrajectoryComponent>(e);
		auto& transform = ecs.get<eNsLogic2DComp::Transform2DComponent>(e);

		trajComp.elapsedTime += deltaTime;


		transform = trajComp.trajectoryFunc(trajComp.elapsedTime);

		std::cout << "entity" << e.id << "\n";

		// eNsDebugLog::LogVector_string("SkillTrajectorySystem.cpp: transform ", transform.positionXZ);
	}
}