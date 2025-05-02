#include "EntityUtils.h"

#include "Core/ECS/Component/NameComponent.h"

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