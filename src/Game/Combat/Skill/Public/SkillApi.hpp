#pragma once
#include "Game/Character/Control/Public/ControlFwd.hpp"
#include "Game/Character/FSM/Public/FSMFwd.hpp"
#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Game/Combat/Skill/Public/SkillFwd.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <span>

namespace Game::Combat::Skill
{
	// private
	void RegisterSkillDef(SkillCatalogBuilder& builder);


	struct SkillRuntimePipelineEntry
	{
		Engine::ECS::Core::Entity e = Engine::ECS::Core::INVALID;

		const Game::Character::FSM::Skill::SkillStateComp* state;
		SkillRuntimeComp* runtimeComp;
		const Game::Character::Control::SkillIntentComponent* intent;
	};
	using SkillRuntimePipeline = std::vector<SkillRuntimePipelineEntry>;

	// private: update all
	void BuildSkillRuntimePipeline(Engine::WorldSystem::Core::WorldCtx& ctx, SkillRuntimePipeline& out);
	
	struct SkillTriggerSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx_;

		void Update(const std::span<SkillRuntimePipelineEntry> ents, const float dt);
	};

	struct SkillRuntimeSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx_;

		void Update(const std::span<SkillRuntimePipelineEntry> ents, const float dt);
	};

	struct SkillLogicCommandSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx_;

		void Update(const std::span<SkillRuntimePipelineEntry> ents, const float dt);
	};

	// public
	void InitAllSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
	void UpdateAllSkillRuntimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

	bool InitSkillSlot(
		Engine::WorldSystem::Core::WorldCtx& ctx,
		SkillSlotLoadoutDTO& dto,
		SkillSlotComponent& outSlots
	);

	bool InitSkillSlot(Engine::WorldSystem::Core::WorldCtx& ctx);

}