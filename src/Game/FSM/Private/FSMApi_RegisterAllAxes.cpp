#include "Game/FSM/Public/FSMApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
//#include "Engine/FSM/Public/Core/Registry.hpp"
#include "Engine/FSM/Public/Base/Registry.hpp"

#include "Game/Character/Private/FSM/Public/FSMApi.hpp"


namespace Game::FSM
{
	using namespace Engine::WorldSystem::Core;
	//using namespace Engine::FSM::Core;
	using namespace Engine::FSM::Base;

	void RegisterAllAxes(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& reg = ctx.ww.GetResource<FSMRegistry>();

		//// 
		//Game::Character::FSM::Movement::RegisterMovementAuthoring(reg);


		Game::Character::FSM::Movement::makeAxis_Movement(reg);
		Game::Character::FSM::Movement::makeFSM_Movement(reg);
	}
}