#pragma once

#include "Engine/ECS/EntityManager.h"




namespace Game::Combat::Skill::System
{
	// スキルのライフタイムを管理するシステム
	// System to manage the lifetime of skills
	class AttackLifetimeSystem
	{
	public:
		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
}