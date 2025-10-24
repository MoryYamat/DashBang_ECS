#include "HitEventFeature.hpp"

#include "Game/Combat/Private/HitEvent/Database/InitHitEventDatabase.hpp"

#include "Game/Combat/Private/HitEvent/System/HitEventManager.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Feature::Combat
{
	using namespace Engine::ECS;

	using namespace Game::Combat;

	// Init
	void HitEventFeature::InitializeHitEventDatabase(EntityMgr& ecs)
	{
		Game::Combat::HitEvent::Init::InitHitEventDatabase(ecs);
	}

	void HitEventFeature::UpdateHitEventManager(EntityMgr& ecs)
	{
		HitEvent::System::HitEventManager(ecs);
	}

	//
	//
	void HitEventFeature::InitializeHitEventDatabase(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		
	}

	void HitEventFeature::UpdateHitEventManager(Engine::WorldSystem::Core::WorldCtx& ctx) 
	{
		
	}
}