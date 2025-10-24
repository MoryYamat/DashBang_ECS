#pragma once
#include "Engine/ECS/EntityManager.h"


namespace Game::Character::State::Action
{
	// キャラクターのスキル実行状態をリセットするシステム
	class CharacterSkillExecutionResetSystem
	{
	public:
		// スキル実行状態をリセットする
		static void ResetSkillExecutionState(Engine::ECS::EntityMgr& ecs, float deltaTime);
	};
} // namespace Game::Character::State::Action