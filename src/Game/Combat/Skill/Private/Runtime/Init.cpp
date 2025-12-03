#include "Game/Combat/Skill/Public/Runtime/SkillRuntimeAPI.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Combat::Skill::Runtime
{
	// Combat/Skill/Private/Core/Init.cppで呼び出し
	void InitAllSkillRuntimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		// コマンドバッファ作成/初期化
		auto& cmds = ctx.ww.CreateResource<SkillLogicCommandBuffer>();
		cmds.clear();
	}
}