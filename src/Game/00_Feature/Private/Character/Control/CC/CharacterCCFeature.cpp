#include "CharacterCCFeature.hpp"

#include "Game/Character/Private/Control/CC/System/CCAntiChainSystem.hpp"

#include "Game/Character/Private/Control/CC/Policy/Init/InitCCAntiChainPolicy.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Feature::Character
{
	using namespace Engine::ECS;
	using namespace Game::Character::Control::CC;



	void CharacterCCFeature::InitCCAntiChainPolicy(Engine::ECS::EntityMgr& ecs)
	{
		Policy::InitCCAntiChainPolicy(ecs);
	}

	void CharacterCCFeature::UpdateCCAntiChainSystem(EntityMgr& ecs)
	{
		CCAntiChainSystem(ecs);
	}

	//
	// 
	void CharacterCCFeature::InitCCAntiChainPolicy(Engine::WorldSystem::Core::WorldCtx& ctx)
	{

	}

	void CharacterCCFeature::UpdateCCAntiChainSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{

	}
}