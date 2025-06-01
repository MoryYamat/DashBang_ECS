// 初期状態を基準とした差分や補正値とTransform2Dの情報によってワールド形状のためのTransform2DComponentを生成する関数群


#pragma once

#include "Game/Combat/Skill/MasterData/SkillDefinition.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include <functional>

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Combat::Skill::Factory
{
	class SkillTrajectoryFactory
	{
	public:
		// SkillDefinition + Origin Transform から軌跡関数を生成
		static std::function<eNsLogic2DComp::Transform2DComponent(float)> Create(
			const gNsSkillData::SkillDefinition& def,
			const eNsLogic2DComp::Transform2DComponent& origin);
	};
}

