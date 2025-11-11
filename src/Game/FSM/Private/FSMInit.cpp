#include "Game/FSM/Public/FSMApi.hpp"
// #include "Engine/FSM/Public/FSMApi.hpp"

#include "Engine/FSM/Public/FSMApi.hpp"
#include "Game/FSM/Private/InitFunctions.hpp"
#include "Game/Character/Private/FSM/Public/FSMApi.hpp"
#include "Game/Character/Private/FSM/Private/Movement/Public/MovementAxis.hpp"

namespace Game::FSM
{
	// using namespace Engine::FSM::Core;

	using namespace Engine::FSM::Core;



	bool InitAllFSMs_Game(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto registerFn = [](FSMRegistry& reg)
			{
				Game::Character::FSM::Movement::RegisterMovementAxes(reg);
			};

		// auto registerFn = MakeGameRegisterProvider();

		auto resolverProvider = MakeGameResolverProvider();

		return InitAllFSMs(ctx, registerFn, resolverProvider, BuildStrictness::Strict);
	}

}