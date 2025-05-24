// Functions that create shape information in the world from local shape information and TransformComponent
// 形状ローカル情報とTransformComponentからワールド上での形状情報を作成する関数群


#pragma once

#include "Core/ECS/Component/Transform2DComponent.h"

#include "Game/SkillSystem/Component/Attack2DAreaComponent.h"

#include <GLM/glm.hpp>

namespace ShapeUtils
{
	// A function that calculates the world shape using local 2D shape information and world transformation information.
	// ローカル2D形状情報とワールド変換情報を用いて，ワールド形状を計算する関数
	Attack2DShape ComputeWorldShape(const Attack2DShape& localShape, const Transform2DComponent& transform);

	Circle2DAttack ComputeWorldCircle(const Circle2DAttack& local, const Transform2DComponent& transform);

	Sector2DAttack ComputeWolrdSector(const Sector2DAttack& local, const Transform2DComponent& transform);

	Rectangle2DAttack ComputeWorldRectangle(const Rectangle2DAttack& local, const Transform2DComponent& transform);

	//glm::vec2 GetWorldPosition();
}