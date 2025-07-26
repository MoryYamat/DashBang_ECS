#include "SkillFSMSystem.hpp"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Game/Combat/Skill/Component/SkillExecutionComponent.hpp"
#include "Game/Combat/Skill/Component/SkillEffectExecutionRecordComponent.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillStateComponent.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMTransitionRequestComponent.hpp"

#include "Game/Combat/Skill/FSM/SkillStateTags.hpp"

#include "Common/GameNamespaceDecl.h"

#include <iostream>
#include <functional>

// TODO: 遷移評価だけなので `break`を消すために，遷移priorityのいい初期化，定義方法を考える
// TODO: 多段遷移を考える
void Game::Combat::Skill::FSM::UpdateSkillFSMSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Game::Combat::Skill::Database;
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::FSM;
	using namespace Game::Combat::Skill::FSM::StateModel;

	auto& db = ecs.getResource<Game::Combat::Skill::Database::SkillDatabase>();

	for (eNsECS::Entity eExec : ecs.view<
		SkillExecutionComponent
	>())
	{
		auto& exec = ecs.get<SkillExecutionComponent>(eExec);
		const auto caster = exec.caster;

		exec.elapsedTime += deltaTime;
		exec.phaseElapsedTime += deltaTime;

		// キャスターが存在していない or 破棄済みならスキップ
		if (!ecs.isAlive(caster))continue;


		// SkillStateComponentがキャスターについているか確認
		if (!ecs.hasComponent<SkillStateComponent>(caster)) continue;


		auto& state = ecs.get<SkillStateComponent>(caster);
		const auto& skillId = exec.skillId;

		// スキルデータベースからSkillEntryを取得
		if (!db.Has(skillId)) continue;
		const auto& entry = db.Get(skillId);
		const auto& fsmDef = entry.fsm;
		const auto& def = entry.def;

		// Context構築
		SkillFSMContext ctx;
		ctx.id = skillId;
		ctx.elapsedTime = exec.elapsedTime;
		ctx.phaseElapsedTime = exec.phaseElapsedTime;
		ctx.isInterrupted = exec.isInterrupted;

		// ==== 状態遷移処理 ====
		for (const auto& transition : fsmDef.transitions)
		{
			
			// from条件があるならチェック(nullopt = すべての状態から許容)
			if (transition.from.has_value() && state.current != transition.from.value())
				continue;

			// std::cout << "here :" << exec.elapsedTime << std::endl;


			if (transition.condition->evaluate(ctx, def))
			{
				auto& reqComp = ecs.get<SkillFSMTransitionRequestComponent>(caster);
				reqComp.requests.push_back(SkillFSMTransitionRequest{
					.requestedTo = transition.to,
					.priority = 0// 現在は固定値（将来は自動化）
					});

				// 遷移を適用
				// state.current = transition.to;
				exec.phaseElapsedTime = 0.0f;

				// ログ
				//std::cout << "[SkillFSMSystem.cpp]: Skill " << skillId << " transitioned to " << state.current.name() << "\n";

				break; // 1ステップで1遷移だけ行う
			}
		}
	}
}
