#include "EntityManager.h"


#include "Engine/ECS/Component/Graphics/MeshComponent.h"



template<typename T>
void destroyIfExists(std::type_index currentType, std::shared_ptr<void>& compPtr)
{
	if (currentType == std::type_index(typeid(T)))
	{
		auto typed = std::static_pointer_cast<T>(compPtr);
		typed->Destroy();
	}
}

// Clear all components at once
void Engine::ECS::EntityMgr::Clear()
{
	for (auto& [typeIndex, componentMap] : mComponentPools)
	{
		for (auto& [entityID, compPtr] : componentMap)
		{
			destroyIfExists<eNsGfxComp::MeshComponent>(typeIndex, compPtr);
		}
	}

	mComponentPools.clear();
}