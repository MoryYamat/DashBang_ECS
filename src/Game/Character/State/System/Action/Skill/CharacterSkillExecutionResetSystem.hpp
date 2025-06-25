#pragma once
#include "Engine/ECS/EntityManager.h"
#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::State::Action
{
	// キャラクターのスキル実行状態をリセットするシステム
	class CharacterSkillExecutionResetSystem
	{
	public:
		// スキル実行状態をリセットする
		static void ResetSkillExecutionState(eNsECS::EntityMgr& ecs, float deltaTime);
	};
} // namespace Game::Character::State::Action