// �R���|�[�l���g�������̔ėp�e���v���[�g
// Generic template for component initialization

#pragma once

#include "Core/ECS/EntityManager.h"

template<typename Component>
struct InitSystem
{
	static void Init(Component& comp, ECS& ecs, Entity& owner)
	{
		// The default is to do nothing.
	}
};