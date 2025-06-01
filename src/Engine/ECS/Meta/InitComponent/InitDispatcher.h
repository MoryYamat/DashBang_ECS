// 
#pragma once

#include "Engine/ECS/Meta/InitComponent/ApplyInitForComponent.h"

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Engine::ECS::Meta::Init
{
	template<typename... Components>
	void ApplyAllDeferredInitializations(eNsECS::EntityMgr& ecs, eNsWindow::Window& window)
	{
		(ApplyDeferredInitialization<Components>(ecs, window), ...);
	}
}
