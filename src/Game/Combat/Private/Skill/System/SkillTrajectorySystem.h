// 毎フレーム Transform2DComponent を更新するシステム
// A system that updates the Transform2DComponent every frame


#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"




namespace Game::Combat::Skill::System
{
	class SkillTrajectorySystem
	{
	public:

		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}

