#include "Game/Character/Private/FSM/Public/SkillAxisApi.hpp"
#include "Game/Character/Private/FSM/Public/SkillTypes.hpp"

#include "Game/Character/Private/FSM/Public/SkillAxisComponent.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"



namespace Game::Character::FSM::Skill
{
	using namespace Engine::WorldSystem::Query;

	void BuildSkillPipeline(Engine::WorldSystem::Core::WorldCtx& ctx, SkillPipeline& out)
	{
		out.clear();
		auto ents = ViewWhere(ctx.rw, All<SkillAxisComp, SkillStateComp>{});

		for (const auto& e : ents)
		{
			auto* axis = ctx.ww.TryGet<SkillAxisComp>(e);
			auto* state = ctx.ww.TryGet<SkillStateComp>(e);


			out.push_back(SkillPipelineEntry{
					.e = e,
					.axis = axis,
					.state = state
				});
		}
	}
}