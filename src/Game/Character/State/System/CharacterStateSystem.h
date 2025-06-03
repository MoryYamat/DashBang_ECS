// キャラクターに共通する意味的状態コンテキストを提供する制御機構

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Character/State/Behavior/CharacterState.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::State
{
	class CharacterStateSystem
	{
	public:

		static void UpdateStates(eNsECS::EntityMgr& ecs, float deltaTime);

		// 状態ユーティリティ
		static void RequestStateChange(eNsECS::EntityMgr& ecs, eNsECS::Entity e, CharacterBehaviorState nextState);
		static void ForceCrowdControl(eNsECS::EntityMgr& ecs, eNsECS::Entity e, CharacterCrowdControlState ccState);
	};
}