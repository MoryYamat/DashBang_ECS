#include "Game/FSM/Public/FSMApi.hpp"
// #include "Engine/FSM/Public/FSMApi.hpp"

#include "Engine/FSM/Public/FSMApi.hpp"

#include "Game/Character/Private/FSM/Public/FSMApi.hpp"

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

		//auto condProvider = [](const FSMCatalog& cat, std::vector<AxisCondBindings>& out)
		//	{
		//		(void)cat;
		//		Game::Character::FSM::Movement::ProvideMovementConds(cat, out);
		//	};


		return InitAllFSMs(ctx, registerFn, /*condProvider,*/ BuildStrictness::Strict);
	}

}