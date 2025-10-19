#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"
#include "Game/Combat/Skill/FSM/SkillStateTags.hpp"

#include "Game/Combat/Skill/Component/SkillExecutionContextComponent.hpp"
#include "Game/Combat/Skill/Component/SkillEffectExecutionRecordComponent.hpp"



#include "IEffectTemplate.hpp"

// TODO: SkillExecutionComponentの初期状態と初期化方法をより良い方法を考える
// ToDo: 複数副作用をまとめる`CompositeEffect`を導入すれば，FSM定義の記述力があがる
// TODO: 汎用的な「Actor状態初期化」の副作用システムを用意する
namespace Game::Combat::Skill::FSM::Effect
{


	// SkillExecution情報をリセット
	struct ResetExecutionStateEffect :IEffectTemplate
	{
		void execute(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity caster,
			const Game::Combat::Skill::Def::SkillDef& def,
			const Game::Combat::Skill::FSM::SkillFSMContext& ctx
		) const override
		{
			using namespace Game::Combat::Skill::Component;

			if (!ecs.hasComponent<SkillExecutionContextComponent>(caster)) return;

			auto& exec = ecs.get<SkillExecutionContextComponent>(caster);
			exec.elapsedTime = 0.0f;
			exec.phaseElapsedTime = 0.0f;
			exec.isInterrupted = false;
			exec.previousState = StateTag::NONE;
			exec.skillId = 0;

			if (ecs.hasComponent<SkillEffectExecutionRecordComponent>(caster))
			{
				auto& record = ecs.get<SkillEffectExecutionRecordComponent>(caster);
				record.clear();// 副作用実行ログをリセット
				std::cout << "[ResetExecutionStateEffect] Cleared effect execution record for caster: " << caster.id << "\n";
			}
		}
	};
}