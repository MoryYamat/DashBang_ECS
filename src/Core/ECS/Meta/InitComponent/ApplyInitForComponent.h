// 一括初期化のための関数テンプレート
// Function template for bulk initialization


#pragma once 

#include "Core/ECS/EntityManager.h"
#include "Core/ECS/Meta/InitPhase.h"
#include "Core/ECS/Meta/InitPhaseSpecialization.h"
#include "Core/ECS/Meta/InitComponent/InitSystem.h"


template<typename Component>
void ApplyDeferredInitialization(ECS& ecs)
{
	constexpr InitPhase phase = GetInitPhase<Component>();

	if constexpr (phase == InitPhase::Deferred)
	{
		std::cout << "[Init] Deferred Init for: " << typeid(Component).name() << std::endl;

		auto entities = ecs.view<Component>();
		std::cout << " - Found " << entities.size() << " entities." << std::endl;

		for (Entity e : ecs.view<Component>())
		{
			auto& comp = ecs.get<Component>(e);
			InitSystem<Component>::Init(comp, ecs, e);
		}
	}
}