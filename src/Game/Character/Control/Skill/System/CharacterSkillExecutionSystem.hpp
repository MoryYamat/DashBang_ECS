#pragma once

#include "Engine/ECS/EntityManager.h"


#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"



namespace Game::Character::Control::Skill
{
	// キャラクターのスキル実行システム
	void UpdateCharacterSkillExecution(Engine::ECS::EntityMgr& ecs, float deltaTime);

}