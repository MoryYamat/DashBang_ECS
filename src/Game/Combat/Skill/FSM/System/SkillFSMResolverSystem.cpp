#include "SkillFSMResolverSystem.hpp"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Game/Combat/Skill/Component/SkillExecutionComponent.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillStateComponent.hpp"
#include "Game/Combat/Skill/Component/SkillEffectExecutionRecordComponent.hpp"


#include "Game/Combat/Skill/FSM/StateModel/SkillFSMTransitionRequestComponent.hpp"

#include <typeindex>

// TODO: リクエストを一定時間キューに保持しておきたい場合や，リクエストに「有効期間」や「依存関係」がある場合.requests.clear()では不可
// TODO: 優先度が同じ場合のルールが未定義(先に来たほうを採用している)
void Game::Combat::Skill::FSM::System::SkillFSMResolverSystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::Database;
	using namespace Game::Combat::Skill::FSM;
	using namespace Game::Combat::Skill::FSM::StateModel;

	auto& db = ecs.getResource<SkillDatabase>();

	for (eNsECS::Entity eExec : ecs.view<SkillExecutionComponent>())
	{
		auto& exec = ecs.get<SkillExecutionComponent>(eExec);
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
				tryTriggerEffect(hook, ecs, eExec, eCaster, entry.def, ctx, state.current, fromState);
			}
		}

		// リクエストを消費
		reqComp.requests.clear();


	}
}

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