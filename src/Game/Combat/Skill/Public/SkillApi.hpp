#pragma once
#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include "Game/Combat/Skill/Public/SkillFwd.hpp"

#include <vector>
#include <span>

namespace Game::Combat::Skill
{
	// private
	void RegisterSkillDef(SkillCatalogBuilder& builder);

	struct SkillRuntimePipelineEntry
	{
		Engine::ECS::Core::Entity e = Engine::ECS::Core::INVALID;

		SkillRuntimeComp* runtimeComp;
	};
	using SkillRuntimePipeline = std::vector<SkillRuntimePipelineEntry>;

	// private: update all
	void BuildSkillRuntimePipeline(Engine::WorldSystem::Core::WorldCtx& ctx, SkillRuntimePipeline& out);
	
	struct SkillTriggerSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(std::span<SkillRuntimePipelineEntry> ents, float dt);
	};

	struct SkillRuntimeSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(std::span<SkillRuntimePipelineEntry> ents, float dt);
	};

	struct SkillLogicCommandSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(std::span<SkillRuntimePipelineEntry> ents, float dt);
	};

	// public
	void InitAllSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
	void UpdateAllSkillRuntimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx);





}