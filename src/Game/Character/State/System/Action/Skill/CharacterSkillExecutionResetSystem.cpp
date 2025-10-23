#include "CharacterSkillExecutionResetSystem.hpp"


#include "Game/Character/State/Component/Action/Skill/CharacterSkillExecutionStateComponent.hpp"



#include "Engine/Debug/Private/DebugUtils.h"

#include <string>


// Fixme: このlifetimeComponentによって時間経過およびリセット機構を再実装する必要あり
void Game::Character::State::Action::CharacterSkillExecutionResetSystem::ResetSkillExecutionState
(
	Engine::ECS::EntityMgr& ecs,
	float deltaTime
)
{
	for (Engine::ECS::Entity ePlayer : ecs.view<Game::Character::State::Action::CharacterSkillExecutionStateComponent>())
	{
		auto& skillExec = ecs.get<Game::Character::State::Action::CharacterSkillExecutionStateComponent>(ePlayer);

		if (skillExec.currentPhase == Game::Character::State::Action::CharacterSkillExecutionPhase::Completed)
		{
			skillExec.currentPhase = Game::Character::State::Action::CharacterSkillExecutionPhase::None;
			skillExec.previousPhase = Game::Character::State::Action::CharacterSkillExecutionPhase::None;
			skillExec.elapsedTime = 0.0f; // 経過時間をリセット
			// デバッグログ
			Engine::Debug::Logging::GeneralLog("CharacterSkillExecutionResetSystem", "Skill execution state reset for entity: " + std::to_string(ePlayer.id));

			//skillExec.elapsedTime += deltaTime;
			//if (skillExec.elapsedTime > 0.2f) // 0.2秒以上経過したらリセット (このタイムラグは不要かもしれない)
			//{
			//	skillExec.currentPhase = Game::Character::State::Action::CharacterSkillExecutionPhase::None;
			//	skillExec.previousPhase = Game::Character::State::Action::CharacterSkillExecutionPhase::None;
			//	skillExec.elapsedTime = 0.0f; // 経過時間をリセット
			//	// デバッグログ
			//	Engine::Debug::Logging::GeneralLog("CharacterSkillExecutionResetSystem", "Skill execution state reset for entity: " + std::to_string(ePlayer.id));
			//}
		}

		// Interrupted状態もリセットする処理を追加予定
		// Interrupted状態もリセットする処理を追加予定
		// Interrupted状態もリセットする処理を追加予定
	}
}