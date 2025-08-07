#include "SkillFSMResolverSystem.hpp"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Game/Combat/Skill/Component/SkillExecutionContextComponent.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillStateComponent.hpp"
#include "Game/Combat/Skill/Component/SkillEffectExecutionRecordComponent.hpp"


#include "Game/Combat/Skill/FSM/StateModel/SkillFSMTransitionRequestComponent.hpp"

#include <typeindex>

// TODO: eExecとeCasterは同じになったので不要な処理あり，改善してもよいが後回し
// TODO: リクエストを一定時間キューに保持しておきたい場合や，リクエストに「有効期間」や「依存関係」がある場合.requests.clear()では不可
// TODO: 優先度が同じ場合のルールが未定義(先に来たほうを採用している)
void Game::Combat::Skill::FSM::System::SkillFSMResolverSystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::Database;
	using namespace Game::Combat::Skill::FSM;
	using namespace Game::Combat::Skill::FSM::StateModel;

	auto& db = ecs.getResource<SkillDatabase>();

	// ここで，`SkillExecution`を検索するのはどうなのか．常駐になったらフラグ的役割を果たせなくなる
	for (eNsECS::Entity eExec : ecs.view<SkillExecutionContextComponent>())
	{
		auto& exec = ecs.get<SkillExecutionContextComponent>(eExec);

		if (exec.skillId == 0) continue;// スキル未実行なのでスキップ

		const auto eCaster = exec.caster;

		exec.elapsedTime += deltaTime;
		exec.phaseElapsedTime += deltaTime;

		if (!ecs.isAlive(eCaster)) continue;
		if (!ecs.hasComponent<SkillStateComponent>(eCaster)) continue;
		if (!ecs.hasComponent<SkillFSMTransitionRequestComponent>(eCaster)) continue;

		auto& state = ecs.get<SkillStateComponent>(eCaster);
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
			const std::type_index fromState = state.current;
			const std::type_index toState = bestRequest->requestedTo.value();

			exec.previousState = fromState;
			// 状態更新
			state.current = toState;
			exec.phaseElapsedTime = 0.0f;

			std::cout << "[SkillFSMResolverSystem] Resolved transition: "
				<< fromState.name() << " -> " << toState.name()
				<< " on Entity " << eCaster.id << "\n";

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

			for (const auto& hook : entry.fsm.effectHooks)
			{
				tryTriggerEffect(hook, ecs, eCaster, entry.def, ctx, state.current, fromState);
			}

			// リセット処理
			tryTriggerReset(entry.fsm, entry.def, ecs, eCaster, ctx, state.current, fromState);
		}

		// リクエストを消費
		reqComp.requests.clear();


	}
}

// FIXME: リセット系のEffectはこのtyrTriggerで処理すると二度とリセットされない．(処理順のせい)
// TODO: 設計を変えるかフラグを使うか
void Game::Combat::Skill::FSM::System::SkillFSMResolverSystem::tryTriggerEffect(
	const SkillEffectHook& hook,
	eNsECS::EntityMgr& ecs,
	eNsECS::Entity caster,
	const SkillDef& def,
	const SkillFSMContext& ctx,
	std::type_index current,
	std::type_index previous
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

	std::cout << "[tryTriggerEffect] effectHash: " << hash << "\n";
	std::cout << "[tryTriggerEffect] hasExecuted: " << record.hasExecuted(hash) << "\n";
	std::cout << "[tryTriggerEffect] trigger.evaluate: " << hook.trigger->evaluate(ctx, def, current, previous) << "\n";

}

void Game::Combat::Skill::FSM::System::SkillFSMResolverSystem::tryTriggerReset
(
	const SkillFSMDefinition& fsm,
	const SkillDef& def,
	eNsECS::EntityMgr& ecs,
	eNsECS::Entity caster,
	const SkillFSMContext& ctx,
	std::type_index current,
	std::type_index previous
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

// 
// FIXME: SkillExecutionComponentの常駐化にともなう設計変更が必要
void Game::Combat::Skill::FSM::System::SkillFSMResolverSystem::tryTriggerEffect(
	const SkillEffectHook& hook,
	eNsECS::EntityMgr& ecs,
	eNsECS::Entity eExec,
	eNsECS::Entity caster,
	const SkillDef& def,
	const SkillFSMContext& ctx,
	std::type_index current,
	std::type_index previous
)
{
	using namespace Game::Combat::Skill::Component;

	if (!ecs.hasComponent<SkillEffectExecutionRecordComponent>(eExec))
	{
		ecs.addComponent(eExec, SkillEffectExecutionRecordComponent{});
	}

	auto& record = ecs.get<SkillEffectExecutionRecordComponent>(eExec);

	// 最適化検討：他の方法がないか
	std::size_t hash = std::type_index(typeid(*hook.effect)).hash_code();// ハッシュ値作成

	// すでにTriggerしたEffectはスキップする
	if (hook.trigger->evaluate(ctx, def, current, previous) &&
		!record.hasExecuted(hash))
	{
		hook.effect->execute(ecs, caster, def, ctx);
		record.markExecuted(hash);
	}
}