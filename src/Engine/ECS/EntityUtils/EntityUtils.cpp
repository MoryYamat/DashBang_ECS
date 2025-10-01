#include "EntityUtils.h"

#include "Engine/ECS/Component/Utils/NameComponent.h"

#include "Engine/ECS/Component/Tags/PendingDestroyComponent.h"

#include <iostream>

Engine::ECS::Entity Engine::ECS::EntityUtils::getEntityByName(EntityMgr& ecs, const std::string& name)
{
	// Efficiency Issues
	// Efficiency Issues
	// Efficiency Issues
	// Efficiency Issues
	for (eNsECS::Entity e : ecs.view<eNsUtilComp::NameComponent>())
	{
		const eNsUtilComp::NameComponent& nameComp = ecs.get<eNsUtilComp::NameComponent>(e);
		if (nameComp.name == name)
		{
			//std::cout << "[EntityUtils.cpp]Entity ID: " << e.id << " Entity Name: " << name << std::endl;
			return e;
		}
	}

	std::cout << "[EntityUtils.cpp]: There is no entity named: [[ "<< name << " ]] ." << std::endl;

	return eNsECS::Entity::INVALID;
}

void Engine::ECS::EntityUtils::MarkForPendingDestroyWithChildren(EntityMgr& ecs, eNsECS::Entity parent, const std::vector<eNsECS::Entity>& children,
	int delayFrames, bool fadeOut)
{
	ecs.addComponent<eNsTagComp::PendingDestroyComponent>(parent, eNsTagComp::PendingDestroyComponent{ delayFrames, fadeOut });
	for (eNsECS::Entity child : children)
	{
		if (ecs.isAlive(child))
		{
			ecs.addComponent<eNsTagComp::PendingDestroyComponent>(child, eNsTagComp::PendingDestroyComponent{ delayFrames, fadeOut });
		}
	}
}

void Engine::ECS::EntityUtils::MarkForPendingDestroy(EntityMgr& ecs, eNsECS::Entity entity, int delayFrames, bool fadeOut)
{
	if (ecs.isAlive(entity))
	{
		ecs.addComponent<eNsTagComp::PendingDestroyComponent>(entity, eNsTagComp::PendingDestroyComponent{ delayFrames, fadeOut });
	}
}