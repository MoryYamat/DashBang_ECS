#include "ECSGeneral.hpp"

#include "Game/ECS/System/LifetimeSystem.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Feature::ECS::GameGeneralSytem::UpdateLifetimeSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// ECSのライフタイムシステムを更新
	gNsECSSystem::UpdateLifetimeSystem(ecs, deltaTime);
}