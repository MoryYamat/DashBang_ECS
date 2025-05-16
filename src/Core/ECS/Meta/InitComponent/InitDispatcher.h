// 
#pragma once

#include "Core/ECS/Meta/InitComponent/ApplyInitForComponent.h"

#include "Core/ECS/EntityManager.h"

template<typename... Components>
void ApplyAllDeferredInitializations(ECS& ecs, Window& window)
{
	(ApplyDeferredInitialization<Components>(ecs, window), ...);
}