#include "TestActor.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/Component/Private/Common/TransformComponent.hpp"
#include "Engine/Component/Private/Logic2D/Transform2DComponent.hpp"
#include "Engine/Component/Private/Logic2D/Velocity2DComponent.hpp"

#include "Game/Character/Private/FSM/Public/MovementAxisComponent.hpp"
#include "Game/Character/Private/FSM/Public/MovementAxisApi.hpp"

#include "Game/Character/Private/Control/Movement/Component/IntentComponent.hpp"

#include "Game/Character/Private/Stats/Public/StatsComponent.hpp"

#include <iostream>

namespace Game::Actor
{
	using namespace Engine::WorldSystem::Core;
	using namespace Engine::Component;

	namespace MFSM = Game::Character::FSM::Movement;

	namespace Ctrl = Game::Character::Control;

	TestActor::TestActor(WorldCtx& ctx)
	{
		Engine::ECS::Core::Entity e = ctx.ww.Create();

		MFSM::MovementStateComp& MFSMstate = ctx.ww.Add<MFSM::MovementStateComp>(e);

		MFSM::MovementAxisComp& MFSMComp = ctx.ww.Add<MFSM::MovementAxisComp>(e);
		if (!MFSM::InitMovementAxis(ctx, MFSMComp))
		{
			std::cout << "[TestActor]: Failed to initialize the movement axis component.\n";
		}

		Ctrl::Movement::MovingIntentComponent& mvint = ctx.ww.Add<Ctrl::Movement::MovingIntentComponent>(e);



		TransformComponent& tr = ctx.ww.Add<TransformComponent>(e);
		Transform2DComponent& tr2d = ctx.ww.Add<Transform2DComponent>(e);
		
		Velocity2DComponent& vel = ctx.ww.Add<Velocity2DComponent>(e);

		std::cout << "[Test Actor]: Test Actor Created.\n";
	}
}