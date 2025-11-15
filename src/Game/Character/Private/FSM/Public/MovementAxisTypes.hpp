// Movement Axis DTO

#pragma once

#include "Engine/Public/EngineFwd.hpp"
#include "Game/Character/Private/FSM/Public/MovementAxisComponent.hpp"

#include <cstdint>
#include <vector>
#include <span>

namespace Game::Character::FSM::Movement
{

	struct MovementCondTable
	{
		// std::uint32_t canMove = Engine::FSM::Core::kInvalidID;
		// std::uint32_t shouldStop = Engine::FSM::Core::kInvalidID;
	};

	struct MovementStateTalbe
	{
		std::uint32_t Idle = Engine::FSM::Core::kInvalidID;
		std::uint32_t Moving = Engine::FSM::Core::kInvalidID;
	};

	struct MovementPipelineEntry
	{
		Engine::ECS::Core::Entity e;

		MovementAxisComp* axis;
		MovementStateComp* state;

	};
	using MovementPipeline = std::vector<MovementPipelineEntry>;

	struct MovementFSMSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(std::span<MovementPipelineEntry> ents, const float dt);
	};

	struct MovementEnvSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(std::span<MovementPipelineEntry> ents, const float dt);
	};

	struct MovementLogicSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(std::span<MovementPipelineEntry> ents, const float dt);
	};
}
