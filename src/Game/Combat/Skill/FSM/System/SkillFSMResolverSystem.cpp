#include "SkillFSMResolverSystem.hpp"

#include "Engine/Time/WorldClock.hpp"

#include "Game/Combat/Skill/Component/SkillEffectExecutionRecordComponent.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMTransitionRequestComponent.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillStateComponent.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMLeaseComponent.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMInterferenceRequestComponent.hpp"

#include "Game/Character/FSM/Interference/Core/Data/FSMInterferenceRequest.hpp"

// TODO: eExecとeCasterは同じになったので不要な処理あり，改善してもよいが後回し
// TODO: リクエストを一定時間キューに保持しておきたい場合や，リクエストに「有効期間」や「依存関係」がある場合.requests.clear()では不可
// TODO: 優先度が同じ場合のルールが未定義(先に来たほうを採用している)
void Game::Combat::Skill::FSM::System::SkillFSMResolverSystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::Database;
	using namespace Game::Combat::Skill::FSM;
	using namespace Game::Combat::Skill::FSM::StateModel;

	const auto& clock = Engine::Time::worldClock(ecs);

	auto& db = ecs.getResource<SkillDatabase>();

	// 
	for (eNsECS::Entity caster : ecs.view<
		SkillExecutionContextComponent,
		SkillFSMTransitionRequestComponent,
		SkillStateComponent,
		SkillFSMLeaseComponent
	>())
	{
		auto& exec = ecs.get<SkillExecutionContextComponent>(caster);
		auto& state = ecs.get<SkillStateComponent>(caster);
		auto& reqs = ecs.get<SkillFSMTransitionRequestComponent>(caster);
		const auto& lease = ecs.get<SkillFSMLeaseComponent>(caster);

		if (tryApplyForcedTransition(ecs, caster, state, lease, exec))
		{
			reqs.requests.clear();
			continue;
		}

		if (exec.skillId == 0) continue;// スキル未実行なのでスキップ

		const auto eCaster = exec.caster;

		exec.elapsedTime += deltaTime;
		exec.phaseElapsedTime += deltaTime;

		if (!ecs.isAlive(eCaster)) continue;
		if (!ecs.hasComponent<SkillStateComponent>(eCaster)) continue;
		if (!ecs.hasComponent<SkillFSMTransitionRequestComponent>(eCaster)) continue;

		// auto& state = ecs.get<SkillStateComponent>(eCaster);
		auto& reqComp = ecs.get<SkillFSMTransitionRequestComponent>(eCaster);

		if (reqComp.requests.empty()) continue;

		const SkillFSMTransitionRequest* bestRequest = nullptr;

		for (const auto& req : reqComp.requests)
		{
			if (!req.requestedTo.has_value()) continue;
			if (!bestRequest || req.priority > bestRequest->priority)
				bestRequest = &req;
		}

		if (bestRequest)
		{
			// API化
			// const std::type_index fromState = state.current;
			// const std::type_index toState = bestRequest->requestedTo.value();

			applyStateUpdate(state, bestRequest->requestedTo.value(), exec);

			// exec.previousState = fromState;
			// 状態更新
			// state.current = toState;
			// exec.phaseElapsedTime = 0.0f;

			// std::cout << "[SkillFSMResolverSystem] Resolved transition: "
			// 	<< fromState.name() << " -> " << toState.name()
			// 	<< " on Entity " << eCaster.id << "\n";

			// 副作用フックの実行
			const auto& skillId = exec.skillId;
			if (!db.Has(skillId)) continue;
			const auto& entry = db.Get(skillId);

			// context 
			SkillFSMContext ctx{
				.id = skillId,
				.elapsedTime = exec.elapsedTime,
				.phaseElapsedTime = exec.phaseElapsedTime,
				.isInterrupted = exec.isInterrupted
			};

			// 副作用処理実行
			for (const auto& hook : entry.fsm.effectHooks)
			{
				tryTriggerEffect(ecs, hook, eCaster, entry.def, ctx, state.current, state.previous);
			}

			// リセット処理
			tryTriggerReset(ecs, eCaster, entry.fsm, entry.def,  ctx, state.current, state.previous);
		}

		// リクエストを消費
		reqComp.requests.clear();


	}
}


void Game::Combat::Skill::FSM::System::SkillFSMResolverSystem::tryTriggerEffect(
	eNsECS::EntityMgr& ecs,
	const SkillEffectHook& hook,
	const eNsECS::Entity caster,
	const SkillDef& def,
	const SkillFSMContext& ctx,
	const std::type_index& current,
	const std::type_index& previous
)
{
	using namespace Game::Combat::Skill::Component;

	if (!ecs.hasComponent<SkillEffectExecutionRecordComponent>(caster))
	{
		ecs.addComponent(caster, SkillEffectExecutionRecordComponent{});
	}

	auto& record = ecs.get<SkillEffectExecutionRecordComponent>(caster);

	std::size_t hash = std::type_index(typeid(*hook.effect)).hash_code();// 型に基づく実行識別子

	if (hook.trigger->evaluate(ctx, def, current, previous) &&// トリガー条件判定と未実行チェック
		!record.hasExecuted(hash))
	{
		hook.effect->execute(ecs, caster, def, ctx);// 実行と実行済みマーク
		record.markExecuted(hash);
	}

	//std::cout << "[tryTriggerEffect] effectHash: " << hash << "\n";
	//std::cout << "[tryTriggerEffect] hasExecuted: " << record.hasExecuted(hash) << "\n";
	//std::cout << "[tryTriggerEffect] trigger.evaluate: " << hook.trigger->evaluate(ctx, def, current, previous) << "\n";

}


// FIXME:この汎用構造の中で`EffectExecutionRecordComponent`のリセットを他関数の定義によって実行するのは気持ちが悪い
// FIXME:デフォルトのリセット関数としての機能で`EffectExecutionRecordComponent`をリセットすることにするならよいかも？
// FIXME:ともかく情報の構造から実装の依存関係を整理する必要がある
void Game::Combat::Skill::FSM::System::SkillFSMResolverSystem::tryTriggerReset
(
	eNsECS::EntityMgr& ecs,
	const eNsECS::Entity caster,
	const SkillFSMDefinition& fsm,
	const SkillDef& def,
	const SkillFSMContext& ctx,
	const std::type_index& current,
	const std::type_index& previous
)
{
	for (const auto& hook : fsm.resetHooks)
	{
		if (hook.trigger->evaluate(ctx, def, current, previous))
		{
			for (const auto& handler : hook.handlers)
			{
				handler->execute(ecs, caster, def, ctx);
			}
		}
	}
}

namespace Game::Combat::Skill::FSM::System
{
	namespace IF = Game::Character::FSM::Interference::Core::Data;
	namespace Database = Game::Combat::Skill::Database;
	namespace StateModel = Game::Combat::Skill::FSM;

	bool SkillFSMResolverSystem::tryApplyForcedTransition
	(
		Engine::ECS::EntityMgr& ecs,
		const Engine::ECS::Entity e,
		Game::Combat::Skill::FSM::StateModel::SkillStateComponent& state,
		const Game::Combat::Skill::FSM::StateModel::SkillFSMLeaseComponent& lease,
		Game::Combat::Skill::Component::SkillExecutionContextComponent& exec
	)
	{
		if (lease.mode != IF::InterferenceMode::ForceTransition) return false;
		if (!lease.forcedState.has_value()) return false;

		const std::type_index target = *lease.forcedState;

		if (applyStateUpdate(state, target, exec))
		{

			// runSkillEffects(ecs, e, state, db);
		}

		return true;
	}

	bool SkillFSMResolverSystem::applyStateUpdate
	(
		Game::Combat::Skill::FSM::StateModel::SkillStateComponent& state,
		std::type_index to,
		Game::Combat::Skill::Component::SkillExecutionContextComponent& exec
	)
	{
		if (state.current == to)return false;
		state.previous = state.current;
		state.current = to;
		exec.reset(state.previous);

		std::cout << "[SkillFSMResovler]: transition from "
			<< state.previous.name() << " to " << state.current.name() << "\n";
		return true;
	}

//	void SkillFSMResolverSystem::runSkillEffects
//	(
//		Engine::ECS::EntityMgr& ecs,
//		const Engine::ECS::Entity e,
//		Game::Combat::Skill::FSM::StateModel::SkillStateComponent& state,
//		Game::Combat::Skill::Component::SkillExecutionContextComponent& exec
//	)
//	{
//		const auto& db = ecs.getResource<Database::SkillDatabase>();
//
//		const auto& skillId = exec.skillId;
//		if(!db.Has(skillId)) return;
//		const auto& entry = db.Get(skillId);
//
//		StateModel::SkillFSMContext ctx
//		{
//			.id = skillId,
//			.elapsedTime = exec.elapsedTime,
//			.phaseElapsedTime = exec.phaseElapsedTime,
//			.isInterrupted = exec.isInterrupted
//		};
//
//		for (const auto& hook : entry.fsm.effectHooks)
//		{
//			tryTriggerEffect(ecs, hook, e, entry.def, ctx, state.current, state.previous);
//		}
//	}
}

// 
// FIXME: SkillExecutionComponentの常駐化にともなう設計変更が必要
//void Game::Combat::Skill::FSM::System::SkillFSMResolverSystem::tryTriggerEffect(
//	const SkillEffectHook& hook,
//	eNsECS::EntityMgr& ecs,
//	eNsECS::Entity eExec,
//	eNsECS::Entity caster,
//	const SkillDef& def,
//	const SkillFSMContext& ctx,
//	std::type_index current,
//	std::type_index previous
//)
//{
//	using namespace Game::Combat::Skill::Component;
//
//	if (!ecs.hasComponent<SkillEffectExecutionRecordComponent>(eExec))
//	{
//		ecs.addComponent(eExec, SkillEffectExecutionRecordComponent{});
//	}
//
//	auto& record = ecs.get<SkillEffectExecutionRecordComponent>(eExec);
//
//	// 最適化検討：他の方法がないか
//	std::size_t hash = std::type_index(typeid(*hook.effect)).hash_code();// ハッシュ値作成
//
//	// すでにTriggerしたEffectはスキップする
//	if (hook.trigger->evaluate(ctx, def, current, previous) &&
//		!record.hasExecuted(hash))
//	{
//		hook.effect->execute(ecs, caster, def, ctx);
//		record.markExecuted(hash);
//	}
// }