#pragma once
#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"

#include "Game/Character/FSM/Public/FSMFwd.hpp"
#include "Game/Character/Control/Public/ControlFwd.hpp"

#include "Game/Combat/Skill/Public/SkillFwd.hpp"

#include <span>

namespace Game::Combat::Skill::Runtime
{
	// private
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

	enum class LogicCommandKind : std::uint8_t
	{
		SpawnHitBox,
		PlayAnim,
	};

	struct SkillLogicCommand
	{
		LogicCommandKind kind;
		Engine::ECS::Core::Entity owner = Engine::ECS::Core::INVALID;
		SkillID skill;
		Engine::FSM::Core::StateID state;
		float effectTime = 0.f;					// 発火したタイミング?
		float lifetime = 0.f;
	};

	// フレームごとの一時リソース
	struct SkillLogicCommandBuffer
	{
		std::vector<SkillLogicCommand> cmds;
		void clear() { cmds.clear(); }
	};

	struct SkillCommandExecSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(const float dt);

	private:
		void HandleSpawnHitBox(const SkillLogicCommand& cmd);
		void HandlePlayerAnim(const SkillLogicCommand& cmd);
	};

	// public
	void InitAllSkillRuntimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
	void UpdateAllSkillRuntimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
}