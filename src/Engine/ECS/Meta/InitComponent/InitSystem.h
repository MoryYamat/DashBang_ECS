// コンポーネント初期化の汎用テンプレート
// Generic template for component initialization

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/Window/Window.h"

#include "Common/EngineNamespaceDecl.h"

namespace Engine::ECS::Meta::Init
{
	template<typename Component>
	struct InitSystem
	{
		static void Init(Component& comp, eNsECS::EntityMgr& ecs, eNsECS::Entity owner, const eNsWindow::Window& window)
		{
			// The default is to do nothing.
		}
	};
}

