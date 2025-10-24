// Functions that create shape information in the world from local shape information and TransformComponent
// 形状ローカル情報とTransformComponentからワールド上での形状情報を作成する関数群


#pragma once

#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Game/Combat/Private/Skill/Component/Attack2DAreaComponent.h"

#include <GLM/glm.hpp>




namespace Game::Combat::Skill::Utils
{
	// A function that calculates the world shape using local 2D shape information and world transformation information.
	// ローカル2D形状情報とワールド変換情報を用いて，ワールド形状を計算する関数
	Game::Combat::Skill::Component::Attack2DShape ComputeWorldShape(const Game::Combat::Skill::Component::Attack2DShape& localShape, const Engine::ECS::Component::Logic2D::Transform2DComponent& transform);

	Game::Combat::Skill::Component::Circle2DAttack ComputeWorldCircle(const Game::Combat::Skill::Component::Circle2DAttack& local, const Engine::ECS::Component::Logic2D::Transform2DComponent& transform);

	Game::Combat::Skill::Component::Sector2DAttack ComputeWolrdSector(const Game::Combat::Skill::Component::Sector2DAttack& local, const Engine::ECS::Component::Logic2D::Transform2DComponent& transform);

	Game::Combat::Skill::Component::Rectangle2DAttack ComputeWorldRectangle(const Game::Combat::Skill::Component::Rectangle2DAttack& local, const Engine::ECS::Component::Logic2D::Transform2DComponent& transform);

	//glm::vec2 GetWorldPosition();
}