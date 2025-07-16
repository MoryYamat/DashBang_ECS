#include "SkillFSMSystem.hpp"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Game/Combat/Skill/Component/SkillExecutionComponent.hpp"
#include "Game/Combat/Skill/FSM/Definition/SkillStateComponent.hpp"

#include "Common/GameNamespaceDecl.h"

#include <iostream>

// TODO: 将来的に直交FSMを統合管理するシステムを実装し，各FSMUpdateSystemからはstate更新リクエストを発信するという形にする
void Game::Combat::Skill::FSM::UpdateSkillFSMSystem(eNsECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Game::Combat::Skill::Database;
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::FSM;
	auto& db = ecs.getResource<Game::Combat::Skill::Database::SkillDatabase>();


	for (eNsECS::Entity eExec : ecs.view<SkillExecutionComponent>())
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

		// 遷移の中で一致するものを探す
		for (const auto& transition : fsmDef.transitions)
		{
			
			// from条件があるならチェック(nullopt = すべての状態から許容)
			if (transition.from.has_value() && state.current != transition.from.value())
				continue;

			// std::cout << "here :" << exec.elapsedTime << std::endl;

			// 条件を評価
			if (transition.condition->evaluate(ctx, def))
			{
				// 遷移を適用
				state.current = transition.to;

				exec.phaseElapsedTime = 0.0f;

				// ログ
				std::cout << "[SkillFSMSystem.cpp]: Skill " << skillId << " transitioned to " << state.current.name() << "\n";


				break; // 1ステップで1遷移だけ行う
			}
		}
	}
}