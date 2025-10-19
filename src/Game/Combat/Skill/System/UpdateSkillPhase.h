// 削除予定：FSMシステム実装後廃止


#pragma once 

#include "Engine/ECS/EntityManager.h"
#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"




namespace Game::Combat::Skill::System
{
	void UpdateSkillPhase(Engine::ECS::EntityMgr& ecs, float deltaTime);

	void UpdateSkillPhase(Engine::ECS::EntityMgr& ecs, float deltaTime, Game::Combat::Skill::Data::SkillDatabase& skillDB);

	bool AllAttacksDestroyed(Engine::ECS::EntityMgr& ecs, const Game::Combat::Skill::Component::SkillInstanceComponent& instance);

	bool IsSkillInterrupted(const Game::Combat::Skill::Component::SkillInstanceComponent& instance, Engine::ECS::EntityMgr& ecs);
}