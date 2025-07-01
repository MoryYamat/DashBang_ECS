// SkillExecutionComponentが特定のphaseに到達したときに，攻撃判定を生成するシステム


#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Combat::Skill::System
{
	void SpawnSkillHitAreas(eNsECS::EntityMgr& ecs);
}