// 初期状態を基準とした差分や補正値とTransform2Dの情報によってワールド形状のためのTransform2DComponentを生成する関数群


#pragma once

#include "Game/SkillSystem/MasterData/SkillDefinition.h"
#include "Core/ECS/Component/Transform2DComponent.h"

#include <functional>

class SkillTrajectoryFactory
{
public:
	// SkillDefinition + Origin Transform から軌跡関数を生成
	static std::function<Transform2DComponent(float)> Create(
		const SkillDefinition& def,
		const Transform2DComponent& origin);
};