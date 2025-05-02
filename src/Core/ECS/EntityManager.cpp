#include "EntityManager.h"


#include "Core/ECS/Component/MeshComponent.h"



template<typename T>
void destroyIfExists(std::type_index currentType, std::shared_ptr<void>& compPtr)
{
	if (currentType == std::type_index(typeid(T)))
	{
		auto typed = std::static_pointer_cast<T>(compPtr);
		typed->Destroy();
	}
}

void ECS::Clear()
{
	for (auto& [typeIndex, componentMap] : mComponentPools)
	{
		for (auto& [entityID, compPtr] : componentMap)
		{
			destroyIfExists<MeshComponent>(typeIndex, compPtr);
		}
	}

	mComponentPools.clear();
}