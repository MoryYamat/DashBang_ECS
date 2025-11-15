#include "Game/Character/Private/FSM/Public/MovementAxisTypes.hpp"
#include "Game/Character/Private/FSM/Public/MovementAxisApi.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/ID-NameHelper.hpp"

#include "Engine/Time/Private/WorldClock.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/FSM/Private/AllFSMSystem.hpp"

#include "Game/Character/Private/Control/Movement/Component/IntentComponent.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <iostream>

namespace Game::Character::FSM::Movement
{
	void MovementEnvSystem::Update(std::span<MovementPipelineEntry> ents, const float dt)
	{
		// std::cout << "here\n";
		for (auto& entry : ents)
		{
			// std::cout << "here\n";
			auto& inst = entry.axis->inst;
			auto& reader = entry.axis->reader;

			inst.beginFrame(dt);

			// 最初は requiredBits や candidateSlots を活かす実装でなくてもよい
			// TODO: 上を活かす実装

			auto* moveIntentDir = ctx.rw.TryGet<Game::Character::Control::Movement::MovingIntentComponent>(entry.e);
			glm::vec2 testDir;
			static float testdt = 0.0f;
			if (testdt < 10.0f)
			{
				testdt += dt;
				testDir = glm::vec2(1.0f);
			}
			else
			{
				testDir = glm::vec2(0.0f);
			}

			// reader.movementInputMag = moveIntentDir ? glm::length2(moveIntentDir->direction) : 0.0f;
			reader.movementInputMag = moveIntentDir ? glm::length2(testDir) : 0.0f;
		}
	}

	void MovementFSMSystem::Update(std::span<MovementPipelineEntry> ents, const float dt)
	{
		for (auto& entry : ents)
		{
			auto& inst = entry.axis->inst;
			auto& reader = entry.axis->reader;

			entry.state->changedThisFrame = false;

			// FSM評価
			Engine::FSM::Core::Decision d = inst.tick(reader);

			if (inst.ApplyDecision(d))
			{
				entry.state->prevState = entry.state->curState;
				entry.state->curState = inst.curState;
				entry.state->changedThisFrame = d.changed;

				Engine::FSM::Debug::PrintFSMInfo(entry.e, inst, d);
			}

		}
	}

	void MovementLogicSystem::Update(std::span<MovementPipelineEntry> ents, const float dt)
	{
		for (auto& entry : ents)
		{
			if (!entry.state) continue;

			const auto cur = entry.state->curState;

			// switchではない方法で実装する
		}
	}

	void UpdateMovementAll(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		MovementPipeline pipeline;
		BuildMovementPipeline(ctx, pipeline);

		// ワールドリソースからとる予定
		const auto& dt = ctx.rw.GetResource<Engine::Time::WorldClockData>();


		MovementEnvSystem envSys{ ctx };
		MovementFSMSystem fsmSys{ ctx };
		MovementLogicSystem logicSys{ ctx };


		envSys.Update(pipeline, dt.fixedDt);
		fsmSys.Update(pipeline, dt.fixedDt);
		logicSys.Update(pipeline, dt.fixedDt);
	}
}