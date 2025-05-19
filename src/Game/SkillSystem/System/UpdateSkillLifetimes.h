// Attack2DAreaComponent�i�ȂǃX�L���̌��ʂƂ��Đ������ꂽ�G���e�B�e�B�j �� lifetime ���Ǘ��E�I�������鏈��

#pragma once

#include "Core/ECS/EntityManager.h"


#include "Game/SkillSystem/MasterData/SkillDatabase.h"

namespace SkillSystem::Lifetime
{
	void UpdateSkillLifetimes(ECS& ecs, float deltaTime, SkillDatabase& skillDB);

	void updateAttack2DAreaCompLifetimes(ECS& ecs, float deltaTime);

	void updateSkillInstanceCompLifetimes(ECS& ecs, float deltaTime, SkillDatabase& skillDB);
}