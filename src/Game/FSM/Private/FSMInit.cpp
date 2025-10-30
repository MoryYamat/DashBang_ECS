#include "Game/FSM/Public/FSMApi.hpp"
#include "Engine/FSM/Public/FSMApi.hpp"


namespace Game::FSM
{
	using namespace Engine::FSM::Core;

	// Init AllFSMs
	void InitAllFSMs(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		InitFSMEngine(ctx);
		// 軸登録
		Game::FSM::RegisterAllAxes(ctx);
		// 軸情報ビルド → FSMCatalogに格納
		BuildCanonicalFSM(ctx);


		// CondTableのリソース確保
		//InitFSMCondTables(ctx);
		InitFSMCondProfiles(ctx);

		// 各軸ごとの条件関数
		//auto stages = Game::FSM::CollectAllCondStages();
		auto perAxisProfile = Game::FSM::CollectAllCondStagesWithProfiles();
		// BuildCondTables(ctx, stages);
		BuildCondProfiles(ctx, perAxisProfile);
	}

}