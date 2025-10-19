#include "CharacterSkillExecutionStateUpdater.hpp"

#include "Game/Character/State/Component/Action/Skill/CharacterSkillExecutionStateComponent.hpp"

#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"



#include "Engine/Debug/DebugUtils.h"


// 廃止予定：SkillSystemを抜本的改善：型駆動FSMを導入
void Game::Character::State::Action::UpdateCharacterSkillExecutionStateFromInstance
(
	Engine::ECS::EntityMgr& ecs,
	float deltaTime
)
{
	for (Engine::ECS::Entity eSkill : ecs.view<Game::Combat::Skill::Component::SkillInstanceComponent>())
	{
		const auto& skillInstance = ecs.get<Game::Combat::Skill::Component::SkillInstanceComponent>(eSkill);
		Engine::ECS::Entity eCaster = skillInstance.caster;

		// キャラクターがSkillExecutionStateComponentを持っているか確認
		if (!ecs.hasComponent<Game::Character::State::Action::CharacterSkillExecutionStateComponent>(eCaster))
			continue;// なければ飛ばす

		auto& skillExec = ecs.get<Game::Character::State::Action::CharacterSkillExecutionStateComponent>(eCaster);

		// 前回の状態を記録
		skillExec.previousPhase = skillExec.currentPhase;

		using Phase = Game::Combat::Skill::Component::SkillPhase;
		using ExecPhase = Game::Character::State::Action::CharacterSkillExecutionPhase;

		// スキルのフェーズに基づいて状態を更新
		// 検討：キャラクターのスキル実行状態に基づいて，スキルインスタンスの状態を更新するべきではないか？
		// 検討：キャラクターのスキル実行状態に基づいて，スキルインスタンスの状態を更新するべきではないか？
		// 検討：キャラクターのスキル実行状態に基づいて，スキルインスタンスの状態を更新するべきではないか？
		switch (skillInstance.phase)
		{
		case Phase::Casting:
			skillExec.currentPhase = ExecPhase::Casting;
			// Engine::Debug::Logging::GeneralLog("CharacterSkillExecutionStateUpdater", "Skill Execution Phase: Casting");
			break;
		case Phase::Active:
			skillExec.currentPhase = ExecPhase::Active;
			// Engine::Debug::Logging::GeneralLog("CharacterSkillExecutionStateUpdater", "Skill Execution Phase: Active");
			break;
		case Phase::Recovery:
			skillExec.currentPhase = ExecPhase::Recovery;
			// Engine::Debug::Logging::GeneralLog("CharacterSkillExecutionStateUpdater", "Skill Execution Phase: Recovery");
			break;
		case Phase::Completed:
			skillExec.currentPhase = ExecPhase::Completed;
			// Engine::Debug::Logging::GeneralLog("CharacterSkillExecutionStateUpdater", "Skill Execution Phase: Completed");
			break;
		case Phase::Interrupted:
			skillExec.currentPhase = ExecPhase::Interrupted;
			// Engine::Debug::Logging::GeneralLog("CharacterSkillExecutionStateUpdater", "Skill Execution Phase: Interrupted");
			break;
		default:
			skillExec.currentPhase = ExecPhase::None;
			// Engine::Debug::Logging::GeneralLog("CharacterSkillExecutionStateUpdater", "Skill Execution Phase: None");
			break;
		}

		skillExec.elapsedTime += deltaTime;
	}
}