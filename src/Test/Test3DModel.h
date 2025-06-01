#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/Graphics/Renderer/Shader.h"

#include "Common/EngineNamespaceDecl.h"


namespace Test::Actor
{
	class Test3DModel
	{
	public:
		Test3DModel(eNsECS::EntityMgr& ecs, eNsGfxRender::Shader* shader);

		~Test3DModel();
	private:

	};
}

