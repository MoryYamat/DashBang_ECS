// Functions that create shape information in the world from local shape information and TransformComponent
// �`�󃍁[�J������TransformComponent���烏�[���h��ł̌`������쐬����֐��Q


#pragma once

#include "Core/ECS/Component/Transform2DComponent.h"

#include "Game/SkillSystem/Component/Attack2DAreaComponent.h"

#include <GLM/glm.hpp>

namespace ShapeUtils
{
	Attack2DShape ComputeWorldShape(const Attack2DShape& localShape, const Transform2DComponent& transform);

	Circle2DAttack ComputeWorldCircle(const Circle2DAttack& local, const Transform2DComponent& transform);

	Sector2DAttack ComputeWolrdSector(const Sector2DAttack& local, const Transform2DComponent& transform);

	Rectangle2DAttack ComputeWorldRectangle(const Rectangle2DAttack& local, const Transform2DComponent& transform);

	//glm::vec2 GetWorldPosition();
}