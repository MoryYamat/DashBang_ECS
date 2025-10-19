#include "ECSGeneral.hpp"

#include "Game/ECS/System/LifetimeSystem.hpp"



void Game::Feature::ECS::GameGeneralSytem::UpdateLifetimeSystem(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	// ECSのライフタイムシステムを更新
	Game::ECS::System::UpdateLifetimeSystem(ecs, deltaTime);
}