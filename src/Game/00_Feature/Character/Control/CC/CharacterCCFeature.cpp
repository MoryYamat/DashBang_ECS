#include "CharacterCCFeature.hpp"

#include "Game/Character/Control/CC/System/CCAntiChainSystem.hpp"

#include "Game/Character/Control/CC/Policy/Init/InitCCAntiChainPolicy.hpp"

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
}