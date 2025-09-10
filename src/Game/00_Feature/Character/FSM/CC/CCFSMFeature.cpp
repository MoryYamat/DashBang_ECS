#include "CCFSMFeature.hpp"

#include "Game/Character/FSM/CC/Init/CCTransitionDefinitionInitialize.hpp"

#include "Game/Character/FSM/CC/System/CCFSMSystem.hpp"
#include "Game/Character/FSM/CC/System/CCFSMResolverSystem.hpp"

namespace Game::Feature::Character::FSM
{
	using namespace Engine::ECS;

	using namespace Game::Character::FSM::CC;

	void CCFSMFeature::InitCCFSMDefinitionDatabase(EntityMgr& ecs)
	{
		InitCCTransitionDefinitionDatabase(ecs);
	}

	void CCFSMFeature::UpdateCCFSMSystem(EntityMgr& ecs)
	{
		System::CCFSMSysmtem(ecs);
	}

	void CCFSMFeature::UpdateCCFSMResolverSystem(EntityMgr& ecs, float deltaTime)
	{
		System::CCFSMResolverSystem::Update(ecs, deltaTime);
	}
}