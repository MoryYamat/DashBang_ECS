#pragma once

#include "Engine/ECS/EntityManager.h"


#include "Common/EngineNamespaceDecl.h"

namespace Game::Combat::Skill::System
{
	// スキルのライフタイムを管理するシステム
	// System to manage the lifetime of skills
	class AttackLifetimeSystem
	{
	public:
		static void Update(eNsECS::EntityMgr& ecs, float deltaTime);
	};
}