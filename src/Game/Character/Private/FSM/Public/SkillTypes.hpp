#pragma once

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"


#include "Game/Character/Private/FSM/Public/SkillAxisComponent.hpp"

#include <vector>
#include <span>

namespace Game::Character::FSM::Skill
{
	struct SkillCondTable
	{
		Engine::FSM::Core::CondID ElapsedTime = Engine::FSM::Core::kInvalidCond;// 軸宇宙
		Engine::FSM::Core::CondID Triggered = Engine::FSM::Core::kInvalidCond;
		bool changedThisFrame = false;
	};

	struct SkillStateTable
	{
		Engine::FSM::Core::StateID None = Engine::FSM::Core::kInvalidState;// 軸宇宙
		Engine::FSM::Core::StateID Casting = Engine::FSM::Core::kInvalidState;
		Engine::FSM::Core::StateID Active = Engine::FSM::Core::kInvalidState;
		Engine::FSM::Core::StateID Recovery = Engine::FSM::Core::kInvalidState;
		Engine::FSM::Core::StateID Completed = Engine::FSM::Core::kInvalidState;

		bool changedThisFrame = false;
	};

	struct SkillProfileTable
	{
		Engine::FSM::Core::ProfileID Default = Engine::FSM::Core::kInvalidProfile;// 軸宇宙
	};

	struct SkillPipelineEntry
	{
		Engine::ECS::Core::Entity e;

		SkillAxisComp* axis;
		SkillStateComp* state;

	};

	using SkillPipeline = std::vector<SkillPipelineEntry>;


	struct SkillEnvSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;
		static void Update(std::span<SkillPipelineEntry> ents, const float dt);
	};

	struct SkillFSMSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		static void Update(std::span<SkillPipelineEntry> ents, const float dt);
	};

	struct SkillLogicSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;
		static void Update(std::span<SkillPipelineEntry> ents, const float dt);
	};
}