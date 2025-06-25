#include "CharacterSkillExecutionResetSystem.hpp"


#include "Game/Character/State/Component/Action/Skill/CharacterSkillExecutionStateComponent.hpp"

#include "Common/GameNamespaceDecl.h"

#include "Engine/Debug/DebugUtils.h"

#include <string>

void Game::Character::State::Action::CharacterSkillExecutionResetSystem::ResetSkillExecutionState
(
	eNsECS::EntityMgr& ecs,
	float deltaTime
)
{
	for (eNsECS::Entity ePlayer : ecs.view<gNsCharaActionState::CharacterSkillExecutionStateComponent>())
	{
		auto& skillExec = ecs.get<gNsCharaActionState::CharacterSkillExecutionStateComponent>(ePlayer);

		if (skillExec.currentPhase == gNsCharaActionState::CharacterSkillExecutionPhase::Completed)
		{
			skillExec.elapsedTime += deltaTime;
			if (skillExec.elapsedTime > 0.2f) // 0.2秒以上経過したらリセット (このタイムラグは不要かもしれない)
			{
				skillExec.currentPhase = gNsCharaActionState::CharacterSkillExecutionPhase::None;
				skillExec.previousPhase = gNsCharaActionState::CharacterSkillExecutionPhase::None;
				skillExec.elapsedTime = 0.0f; // 経過時間をリセット
				// デバッグログ
				eNsDebugLog::GeneralLog("CharacterSkillExecutionResetSystem", "Skill execution state reset for entity: " + std::to_string(ePlayer.id));
			}
		}

		// Interrupted状態もリセットする処理を追加予定
		// Interrupted状態もリセットする処理を追加予定
		// Interrupted状態もリセットする処理を追加予定
	}
}