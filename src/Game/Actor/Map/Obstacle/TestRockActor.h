//

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/Graphics/Renderer/Shader.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Actor::Map
{
	class TestRockActor
	{
	public:
		TestRockActor(eNsECS::EntityMgr& ecs, eNsGfxRender::Shader* shader);

	private:
	};
}
