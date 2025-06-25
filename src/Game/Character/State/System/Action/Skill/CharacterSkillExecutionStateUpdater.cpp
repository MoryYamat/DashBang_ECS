#include "CharacterSkillExecutionStateUpdater.hpp"

#include "Game/Character/State/Component/Action/Skill/CharacterSkillExecutionStateComponent.hpp"

#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"

#include "Common/GameNamespaceDecl.h"

#include "Engine/Debug/DebugUtils.h"

void Game::Character::State::Action::UpdateCharacterSkillExecutionStateFromInstance
(
	eNsECS::EntityMgr& ecs,
	float deltaTime
)
{
	for (eNsECS::Entity eSkill : ecs.view<gNsSkillComp::SkillInstanceComponent>())
	{
		const auto& skillInstance = ecs.get<gNsSkillComp::SkillInstanceComponent>(eSkill);
		eNsECS::Entity eCaster = skillInstance.caster;

		// キャラクターがSkillExecutionStateComponentを持っているか確認
		if (!ecs.hasComponent<gNsCharaActionState::CharacterSkillExecutionStateComponent>(eCaster))
			continue;// なければ飛ばす

		auto& skillExec = ecs.get<gNsCharaActionState::CharacterSkillExecutionStateComponent>(eCaster);

		// 前回の状態を記録
		skillExec.previousPhase = skillExec.currentPhase;

		using Phase = gNsSkillComp::SkillPhase;
		using ExecPhase = gNsCharaActionState::CharacterSkillExecutionPhase;

		// スキルのフェーズに基づいて状態を更新
		// 検討：キャラクターのスキル実行状態に基づいて，スキルインスタンスの状態を更新するべきではないか？
		// 検討：キャラクターのスキル実行状態に基づいて，スキルインスタンスの状態を更新するべきではないか？
		// 検討：キャラクターのスキル実行状態に基づいて，スキルインスタンスの状態を更新するべきではないか？
		switch (skillInstance.phase)
		{
		case Phase::Casting:
			skillExec.currentPhase = ExecPhase::Casting;
			// eNsDebugLog::GeneralLog("CharacterSkillExecutionStateUpdater", "Skill Execution Phase: Casting");
			break;
		case Phase::Active:
			skillExec.currentPhase = ExecPhase::Active;
			// eNsDebugLog::GeneralLog("CharacterSkillExecutionStateUpdater", "Skill Execution Phase: Active");
			break;
		case Phase::Recovery:
			skillExec.currentPhase = ExecPhase::Recovery;
			// eNsDebugLog::GeneralLog("CharacterSkillExecutionStateUpdater", "Skill Execution Phase: Recovery");
			break;
		case Phase::Completed:
			skillExec.currentPhase = ExecPhase::Completed;
			// eNsDebugLog::GeneralLog("CharacterSkillExecutionStateUpdater", "Skill Execution Phase: Completed");
			break;
		case Phase::Interrupted:
			skillExec.currentPhase = ExecPhase::Interrupted;
			// eNsDebugLog::GeneralLog("CharacterSkillExecutionStateUpdater", "Skill Execution Phase: Interrupted");
			break;
		default:
			skillExec.currentPhase = ExecPhase::None;
			// eNsDebugLog::GeneralLog("CharacterSkillExecutionStateUpdater", "Skill Execution Phase: None");
			break;
		}

		skillExec.elapsedTime += deltaTime;
	}
}