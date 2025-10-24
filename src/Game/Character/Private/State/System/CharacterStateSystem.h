//// キャラクターに共通する意味的状態コンテキストを提供する制御機構
//
//#pragma once
//
//#include "Engine/ECS/EntityManager.h"
//
//#include "Game/Character/Private/State/Behavior/CharacterState.h"
//
//
//
//namespace Game::Character::State
//{
//
//
//	class CharacterStateSystem
//	{
//	public:
//
//		static void UpdateStates(Engine::ECS::EntityMgr& ecs, float deltaTime);
//
//		// 状態ユーティリティ
//		static void RequestStateChange(Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity e, CharacterBehaviorState nextState);
//		static void ForceCrowdControl(Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity e, CharacterCrowdControlState ccState);
//	};
//}