#include "CCFSMFeature.hpp"

#include "Game/Character/Private/FSM/CC/Init/CCTransitionDefinitionInitialize.hpp"

#include "Game/Character/Private/FSM/CC/System/CCFSMSystem.hpp"
#include "Game/Character/Private/FSM/CC/System/CCFSMResolverSystem.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

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


namespace Game::Feature::Character::FSM
{
	void CCFSMFeature::InitCCFSMDefinitionDatabase(Engine::WorldSystem::Core::WorldCtx& ctx)
	{

	}

	void CCFSMFeature::UpdateCCFSMSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{

	}

	void CCFSMFeature::UpdateCCFSMResolverSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{

	}
}