#include "Game/Combat/Skill/Public/SkillApi.hpp"

#include "Engine/Time/Private/WorldClock.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Combat/Skill/Public/SkillTypes.hpp"

#include "Engine/Log/Public/LogApi.hpp"

#include <span>

namespace Game::Combat::Skill
{
	using namespace Engine::WorldSystem::Query;

	// 処理対象entity の一括抽出
	void BuildSkillRuntimePipeline(Engine::WorldSystem::Core::WorldCtx& ctx, SkillRuntimePipeline& out)
	{
		out.clear();
		auto ents = ViewWhere(ctx.rw, All<SkillRuntimeComp>{});

		for (const auto& e : ents)
		{
			auto* runtime = ctx.ww.TryGet<SkillRuntimeComp>(e);

			out.push_back(SkillRuntimePipelineEntry{
				 .e = e,
				 .runtimeComp = runtime
				});
		}
	}


	// 現在のSkillRuntimeCompを更新する(トリガー/ 経過時間(ElapsedTime)/ など
	// SkillRuntimeComp -> SkillFSM(Env)
	void SkillTriggerSystem::Update(std::span<SkillRuntimePipelineEntry> ents, float dt)
	{
		for (auto& e : ents)
		{
			//std::cout << "here\n";
		}
	}

	void SkillRuntimeSystem::Update(std::span<SkillRuntimePipelineEntry> ents, float dt)
	{

	}

	void SkillLogicCommandSystem::Update(std::span<SkillRuntimePipelineEntry> ents, float dt)
	{

	}


	// すべてのSkillRuntimeSystemを更新
	void UpdateAllSkillRuntimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		// struct // intent->skilltrigger
		// struct // update runtimecomp
		// struct // update effect commands
		SkillRuntimePipeline pipeline;
		BuildSkillRuntimePipeline(ctx, pipeline);
		
		const auto& clock = ctx.rw.GetResource<Engine::Time::WorldClockData>();

		SkillTriggerSystem trSys{ ctx };
		SkillRuntimeSystem rtSys{ ctx };
		SkillLogicCommandSystem LCSys{ ctx };

		trSys.Update(pipeline, clock.fixedDt);
		rtSys.Update(pipeline, clock.fixedDt);
		LCSys.Update(pipeline, clock.frameDt);
	}
}