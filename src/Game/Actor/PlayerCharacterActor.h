// test player character actor
// Class that creates an entity

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/Graphics/Renderer/Shader.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Game::Actor::Player
{
	class PlayerCharacter
	{
	public:
		PlayerCharacter(eNsECS::EntityMgr& ecs, eNsGfxRender::Shader* shader);

		~PlayerCharacter();

	private:


	};
}

