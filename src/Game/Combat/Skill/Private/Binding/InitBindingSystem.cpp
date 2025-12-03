#include "Game/Combat/Skill/Public/Binding/API.hpp"
#include "Game/Combat/Skill/Private/Binding/BindingBuilder.hpp"

#include "Game/Combat/HitBox/Public/HitBoxTypes.hpp"
#include "Game/Combat/Skill/Public/Core/SkillTypes.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/FSM/Public/Core/Types.hpp"

#include "Engine/Log/Public/LogAPI.hpp"

namespace
{
	using namespace Game::Combat::Skill::Binding;
}


namespace Game::Combat::Skill::Binding
{
	using namespace Engine::FSM::Core;

	using namespace Game::Combat::Skill::Binding;
	using namespace Game::Combat::Skill;
	using namespace Game::Combat::HitBox;

	// Skill 関連コンポーネントがすべて初期化された後に初期化
	void InitAllSkillBindingSysmte(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		// 登録済み前提
		const auto& skill = ctx.ww.GetResource<SkillCatalog>();
		const auto& hitbox = ctx.ww.GetResource<HitBoxCatalog>();
		const auto& fsm = ctx.ww.GetResource<FSMCatalog>();
		
		// Builder 初期化
		SkillBindingBuilder builder(skill, hitbox, fsm);

		// DTO登録
		RegisterSkillBindingDTO(builder);

		// Build
		SkillBindingBuildResult result = builder.Build();

		// エラー出力
		if (!result.errs.ok())
		{
			for (const auto& err : result.errs.msgs)
			{
				Engine::Log::Write(Engine::Log::Level::Error, 
					"SkillBindingBuild",
					err);
			}
		}

		// bind データ を グローバルリソースへ登録
		auto& data = ctx.ww.CreateResource<SkillBindingData>();
		data = result.data;

		// 

		// InitSkillBindingBuilder(builder);
	}
} 