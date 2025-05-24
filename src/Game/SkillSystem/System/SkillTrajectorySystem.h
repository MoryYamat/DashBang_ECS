// 毎フレーム Transform2DComponent を更新するシステム
// A system that updates the Transform2DComponent every frame


#pragma once

#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Component/Transform2DComponent.h"
#include "Game/SkillSystem/Component/SkillTrajectoryComponent.h"

class SkillTrajectorySystem
{
public:
	static void Update(ECS& ecs, float deltaTime);
};