// 毎フレーム Transform2DComponent を更新するシステム
// A system that updates the Transform2DComponent every frame


#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Combat::Skill::System
{
	class SkillTrajectorySystem
	{
	public:

		static void Update(eNsECS::EntityMgr& ecs, float deltaTime);
	};
}

