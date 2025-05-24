// ���t���[�� Transform2DComponent ���X�V����V�X�e��
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