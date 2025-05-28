#include "EntityUtils.h"

#include "Core/ECS/Component/NameComponent.h"

#include "Core/ECS/Component/Tags/PendingDestroyComponent.h"

#include <iostream>

Entity EntityUtils::getEntityByName(ECS& ecs, const std::string& name)
{
	// Efficiency Issues
	// Efficiency Issues
	// Efficiency Issues
	// Efficiency Issues
	for (Entity e : ecs.view<NameComponent>())
	{
		const NameComponent& nameComp = ecs.get<NameComponent>(e);
		if (nameComp.name == name)
		{
			//std::cout << "[EntityUtils.cpp]Entity ID: " << e.id << " Entity Name: " << name << std::endl;
			return e;
		}
	}

	std::cout << "[EntityUtils.cpp]: There is no entity named: [[ "<< name << " ]] ." << std::endl;

	return Entity::INVALID;
}

void EntityUtils::MarkForPendingDestroyWithChildren(ECS& ecs, Entity parent, const std::vector<Entity>& children,
	int delayFrames, bool fadeOut)
{
	ecs.addComponent(parent, PendingDestroyComponent{ delayFrames, fadeOut });
	for (Entity child : children)
	{
		if (ecs.isAlive(child))
		{
			ecs.addComponent(child, PendingDestroyComponent{ delayFrames, fadeOut });
		}
	}
}

void EntityUtils::MarkForPendingDestroy(ECS& ecs, Entity entity, int delayFrames, bool fadeOut)
{
	if (ecs.isAlive(entity))
	{
		ecs.addComponent(entity, PendingDestroyComponent{ delayFrames, fadeOut });
	}
}