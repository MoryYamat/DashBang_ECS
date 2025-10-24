#include "ECSGeneral.hpp"

#include "Game/ECS/Private/System/LifetimeSystem.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"


void Game::Feature::ECS::GameGeneralSytem::UpdateLifetimeSystem(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	// ECSのライフタイムシステムを更新
	Game::ECS::System::UpdateLifetimeSystem(ecs, deltaTime);
}

//
//
namespace Game::Feature::ECS
{
	void GameGeneralSytem::UpdateLifetimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{

	}
}