// Trajectory data

#pragma once

#include <functional>
#include "Core/ECS/Component/Transform2DComponent.h"

#include "Game/SkillSystem/MasterData/SkillDefinition.h"

struct SkillTrajectoryComponent
{
	// �i�s���̌o�ߎ���
	float elapsedTime = 0.0f;

	// �O�Ղ̒�`(�^���C����̎��)
	TrajectoryType type = TrajectoryType::None;

	// ���s���̋�̓I���� (Transform�����ԂŕԂ�)
	std::function<Transform2DComponent(float)> trajectoryFunc;
};