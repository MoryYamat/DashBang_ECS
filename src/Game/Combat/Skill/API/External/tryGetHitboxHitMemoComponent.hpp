#pragma once 

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"


#include "Game/Combat/Skill/Component/HitboxHitMemoComponent.hpp"

namespace Game::Combat::Skill::API::External
{
	inline Game::Combat::Skill::Component::HitboxHitMemoComponent* tryGetMemo
	(
		Engine::ECS::EntityMgr& ecs,
		const Engine::ECS::Entity e
	)
	{
		if (ecs.hasComponent<Game::Combat::Skill::Component::HitboxHitMemoComponent>(e))
		{
			return &ecs.get<Game::Combat::Skill::Component::HitboxHitMemoComponent>(e);
		}
		return nullptr;
	}
}