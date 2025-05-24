// ������Ԃ���Ƃ���������␳�l��Transform2D�̏��ɂ���ă��[���h�`��̂��߂�Transform2DComponent�𐶐�����֐��Q


#pragma once

#include "Game/SkillSystem/MasterData/SkillDefinition.h"
#include "Core/ECS/Component/Transform2DComponent.h"

#include <functional>

class SkillTrajectoryFactory
{
public:
	// SkillDefinition + Origin Transform ����O�Պ֐��𐶐�
	static std::function<Transform2DComponent(float)> Create(
		const SkillDefinition& def,
		const Transform2DComponent& origin);
};