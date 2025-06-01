// Functions that create shape information in the world from local shape information and TransformComponent
// 形状ローカル情報とTransformComponentからワールド上での形状情報を作成する関数群


#pragma once

#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include <GLM/glm.hpp>

#include "Common/EngineNamespaceDecl.h"
#include "Common//GameNamespaceDecl.h"

namespace Game::Combat::Skill::Utils
{
	// A function that calculates the world shape using local 2D shape information and world transformation information.
	// ローカル2D形状情報とワールド変換情報を用いて，ワールド形状を計算する関数
	gNsSkillComp::Attack2DShape ComputeWorldShape(const gNsSkillComp::Attack2DShape& localShape, const eNsLogic2DComp::Transform2DComponent& transform);

	gNsSkillComp::Circle2DAttack ComputeWorldCircle(const gNsSkillComp::Circle2DAttack& local, const eNsLogic2DComp::Transform2DComponent& transform);

	gNsSkillComp::Sector2DAttack ComputeWolrdSector(const gNsSkillComp::Sector2DAttack& local, const eNsLogic2DComp::Transform2DComponent& transform);

	gNsSkillComp::Rectangle2DAttack ComputeWorldRectangle(const gNsSkillComp::Rectangle2DAttack& local, const eNsLogic2DComp::Transform2DComponent& transform);

	//glm::vec2 GetWorldPosition();
}