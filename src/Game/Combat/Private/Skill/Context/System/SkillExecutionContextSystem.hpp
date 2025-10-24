#pragma once

#include "Engine/ECS/EntityManager.h"



namespace Game::Combat::Skill::Context
{
	class SkillExecutionContextSystem
	{
	public:
		void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}