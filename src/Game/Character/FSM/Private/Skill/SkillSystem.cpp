#include "Game/Character/FSM/Public/SkillAxisApi.hpp"
#include "Game/Character/FSM/Public/SkillTypes.hpp"
#include "Game/Character/FSM/Public/SkillAxisComponent.hpp"


#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Time/Private/WorldClock.hpp"


namespace Game::Character::FSM::Skill
{

	void SkillEnvSystem::Update(std::span<SkillPipelineEntry> ents, const float dt)
	{
		for (auto& entry : ents)
		{
			auto& inst = entry.axis->inst;
			auto& reader = entry.axis->reader;

			inst.beginFrame(dt);

			// inst.setBase
			
		}
	}

	void SkillFSMSystem::Update(std::span<SkillPipelineEntry> ents, const float dt)
	{
		for (auto& e : ents)
		{

		}
	}

	void SkillLogicSystem::Update(std::span<SkillPipelineEntry> ents, const float dt)
	{
		for (auto& e : ents)
		{

		}
	}

	void UpdateAllSkillFSM(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		SkillPipeline pipeline;
		BuildSkillPipeline(ctx, pipeline);


		const auto& clock = ctx.rw.GetResource<Engine::Time::WorldClockData>();

		SkillEnvSystem env{ ctx };
		SkillFSMSystem fsm{ ctx };
		SkillLogicSystem logic{ ctx };


		env.Update(pipeline, clock.fixedDt);
	}
}