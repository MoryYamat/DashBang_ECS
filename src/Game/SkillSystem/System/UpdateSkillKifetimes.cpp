//
#include "UpdateSkillLifetimes.h"

#include "Game/SkillSystem/Component/Attack2DAreaComponent.h"
#include "Game/SkillSystem/Component/SkillInstanceComponent.h"

#include <iostream>

#include <vector>

void SkillSystem::Lifetime::UpdateSkillLifetimes(ECS& ecs, float deltaTime, SkillDatabase& skillDB)
{
	SkillSystem::Lifetime::updateAttack2DAreaCompLifetimes(ecs, deltaTime);
	SkillSystem::Lifetime::updateSkillInstanceCompLifetimes(ecs, deltaTime, skillDB);
}

void SkillSystem::Lifetime::updateAttack2DAreaCompLifetimes(ECS& ecs, float deltaTime)
{
	std::vector<Entity> toDestroy;

	for (Entity e : ecs.view<Attack2DAreaComponent>())
	{
		auto& area = ecs.get<Attack2DAreaComponent>(e);
		// lifetime ÇçXêV
		area.lifetime -= deltaTime;

		// std::cout << "[UpdateSkillLifetimes.cpp(update)] skill lifetme: " << area.lifetime << std::endl;

		// lifetimeÇ™êÿÇÍÇΩEntityÇé˚èW
		if (area.lifetime <= 0.0f)
		{
			toDestroy.push_back(e);
		}
	}

	// äYìñEntity ÇÇ∑Ç◊ÇƒçÌèú
	for (Entity e : toDestroy)
	{
		std::cout << "[SkillLifetime] SkillInstanceComponent expired and removed: " << e.id << std::endl;
		ecs.destroyEntity(e);
	}
}

void SkillSystem::Lifetime::updateSkillInstanceCompLifetimes(ECS& ecs, float deltaTime, SkillDatabase& skillDB)
{
	std::vector<Entity> toDestroy;

	for (Entity e : ecs.view<SkillInstanceComponent>())
	{
		auto& skillInstance = ecs.get<SkillInstanceComponent>(e);
		skillInstance.timeSinceCast += deltaTime;

		const SkillDefinition& def = skillDB.Get(skillInstance.skillId);

		if (skillInstance.timeSinceCast >= def.duration)
		{
			toDestroy.push_back(e);
		}
	}

	for (Entity e: toDestroy)
	{
		{
			std::cout << "[SkillLifetime] SkillInstanceComponent expired and removed: " << e.id << std::endl;
			ecs.destroyEntity(e);
		} 
	}
}