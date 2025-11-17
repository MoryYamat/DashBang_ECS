#include "Game/Character/Private/FSM/Public/MovementTypes.hpp"
#include "Game/Character/Private/FSM/Public/MovementAxisApi.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/ID-NameHelper.hpp"
#include "Engine/FSM/Public/Core/StateEvents.hpp"

#include "Engine/Time/Private/WorldClock.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/FSM/Private/AllFSMSystem.hpp"

#include "Game/Character/Private/Control/Movement/Component/IntentComponent.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <iostream>

namespace Game::Character::FSM::Movement
{
	using namespace Engine::FSM::Core;

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
			if (testdt > 5.0f)
			{
				testDir = glm::vec2(0.0f);
			}
			else if (testdt < 10.0f)
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
				const auto& fsm = *inst.fsm;
				const Engine::FSM::Core::StateID s_gid = fsm.local2GlobalState[inst.curState];
				const Engine::FSM::Core::ProfileID p_gid = fsm.local2GlobalProfile[inst.curProfile];
				//entry.state->prevState = entry.state->curState;
				//entry.state->curState = gid;
				//entry.state->changedThisFrame = d.changed;

				entry.state->Transition(entry.state->curState, s_gid, entry.state->curProf, p_gid);

				Engine::FSM::Debug::PrintFSMInfo(entry.e, inst, d);
			}

		}
	}


	void ExecuteMovementOps(std::uint32_t mask, Engine::WorldSystem::Core::WorldCtx& ctx,
		const MovementPipelineEntry& ent, float dt)
	{
		// 
		if (mask & OpBit(MovementOpKind::ApplyVelocityFromIntent))
		{
			// intent -> velocity
			SetMovementVelComp(ent, ctx);
		}
		if (mask & OpBit(MovementOpKind::ZeroVelocity))
		{
			// TODO: velocity -> 0
			SetZeroVel(ent, ctx);
		}

		// 将来その他のOpKind::についての処理を追加
	}

	void MovementLogicSystem::Update(std::span<MovementPipelineEntry> ents, const float dt)
	{
		auto* ltbl = ctx.rw.TryGetResource<MovementLogicTable>();

		if (!ltbl) return;

		for (auto& entry : ents)
		{
			auto* st = entry.state;
			if (!entry.state) continue;

			StateEventView sev(entry.state->prevState, entry.state->curState, entry.state->changedThisFrame);

			for (const auto& rule : ltbl->entries)
			{
				// state一致 & profile一致
				if (rule.key.state != st->curState) continue;
				if (rule.key.profile != st->curProf) continue;

				// Trigger種別に応じてフィルタ
				bool fire = false;
				switch (rule.key.trigger)
				{
				case StateTriggerKind::WhileIn:
					fire = sev.whileIn(rule.key.state);
					break;
				case StateTriggerKind::OnEnter:
					fire = sev.onEnter(rule.key.state);
					break;
				case StateTriggerKind::OnExit:
					fire = sev.onExit(rule.key.state);
					break;
				}

				if (!fire) continue;

				ExecuteMovementOps(rule.opMask, ctx, entry, dt);
			}


			entry.state->ResetChanged();
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