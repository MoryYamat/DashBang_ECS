// 初期状態を基準とした差分や補正値とTransform2Dの情報によってワールド形状のためのTransform2DComponentを生成する関数群


#pragma once

#include "Game/Combat/Private/Skill/MasterData/SkillDefinition.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include <functional>




namespace Game::Combat::Skill::Factory
{
	class SkillTrajectoryFactory
	{
	public:
		// SkillDefinition + Origin Transform から軌跡関数を生成
		static std::function<Engine::ECS::Component::Logic2D::Transform2DComponent(float)> Create(
			const Game::Combat::Skill::Data::SkillDefinition& def,
			const Engine::ECS::Component::Logic2D::Transform2DComponent& origin);
	};
}

