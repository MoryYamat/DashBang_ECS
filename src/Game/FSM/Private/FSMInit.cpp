#include "Game/FSM/Public/FSMApi.hpp"
// #include "Engine/FSM/Public/FSMApi.hpp"

#include "Engine/FSM/Public/Base/FSMApi.hpp"

#include "Game/Character/Private/FSM/Public/FSMApi.hpp"

namespace Game::FSM
{
	// using namespace Engine::FSM::Core;

	using namespace Engine::FSM::Base;

	//// Init AllFSMs
	//void InitAllFSMs(Engine::WorldSystem::Core::WorldCtx& ctx)
	//{
	//	//InitFSMEngine(ctx);
	//	//// 軸登録
	//	//Game::FSM::RegisterAllAxes(ctx);
	//	//// 軸情報ビルド → FSMCatalogに格納
	//	//BuildCanonicalFSM(ctx);


	//	//// CondTableのリソース確保
	//	////InitFSMCondTables(ctx);
	//	//InitFSMCondProfiles(ctx);

	//	// 各軸ごとの条件関数
	//	//auto stages = Game::FSM::CollectAllCondStages();
	//	//auto perAxisProfile = Game::FSM::CollectAllCondStagesWithProfiles();
	//	// BuildCondTables(ctx, stages);
	//	//BuildCondProfiles(ctx, perAxisProfile);

	//	InitFSMEngine(ctx);
	//	RegisterAllAxes(ctx);


	//	// BuildCanonicalFSM(ctx);
	//}

	bool InitAllFSMs_Game(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto registerFn = [](FSMRegistry& reg)
			{
				Game::Character::FSM::Movement::RegisterMovementAxes(reg);
			};

		auto condProvider = [](const FSMCatalog& cat, std::vector<AxisCondBindings>& out)
			{
				(void)cat;
				Game::Character::FSM::Movement::ProvideMovementConds(cat, out);
			};


		return InitAllFSMs(ctx, registerFn, condProvider, BuildStrictness::Strict);
	}

}