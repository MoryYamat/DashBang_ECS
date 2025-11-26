#include "Game/Combat/Skill/Public/SkillApi.hpp"
#include "Game/Combat/Skill/Public/SkillEffectTypes.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Combat/Skill/Public/Builder.hpp"

namespace Game::Combat::Skill
{
	using namespace Engine::WorldSystem::Core;

	void InitAllSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& builder = ctx.ww.CreateResource<SkillCatalogBuilder>();
		
		RegisterSkillDef(builder);

		// コマンドバッファ作成/初期化
		auto& cmds = ctx.ww.CreateResource<SkillLogicCommandBuffer>();
		cmds.clear();
	}
}