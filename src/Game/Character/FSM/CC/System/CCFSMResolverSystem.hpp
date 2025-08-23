#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::FSM::CC::System
{

	class CCFSMResolverSystem
	{
	public:

		void Update(eNsECS::EntityMgr& ecs, float deltaTime);


	private:

		void resolveCCFSMState();
	};

	// void CCFSMResolverSystem(eNsECS::EntityMgr& ecs, float deltaTime);
}