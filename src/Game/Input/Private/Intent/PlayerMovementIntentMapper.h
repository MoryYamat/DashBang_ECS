// 意味的入力(InputAction)をキャラクターの意図(Intent)に変換するシステム

#pragma once 

// #include "Engine/ECS/EntityManager.h"

// #include "Game/Input/InputActionComponent.h"


// #include "Engine/ECS/Component/Input/AnalogInputComponent.h"
// #include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"


// #include "Game/Character/Private/Control/Movement/Component/Intent/MovementIntentComponent.h"
// #include "Game/Character/Private/Control/Movement/Component/Intent/FacingIntentComponent.h"

//
// #include "Game/Input/Public/InputFwd.hpp"

// 
// #include "Engine/Component/Public/ComponentFwd.hpp"
// #include "Engine/InputManager/Public/InputFwd.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"




namespace Game::Input::Intent
{

	struct MovementIntentMappingSystem
	{
		// 入力状態をもとにIntentコンポーネントへ反映する
		// static void UpdatePlayerMovementIntent(Engine::ECS::EntityMgr& ecs);
		// 
		// // ワールド基準移動 (Global X-Z)
		// static void updatePlayerMovementIntent(
		// 	Game::Character::Intent::MovementIntentComponent& intent
		// 	, Game::Input::InputActionComponent& input
		// );
		// 
		// // mouse input -> intent facing
		// static void updatePlayerDirectionIntent(
		// 	Engine::ECS::Component::Input::AnalogInputComponent& analog,
		// 	Game::Character::Intent::FacingIntentComponent& facing,
		// 	Engine::ECS::Component::Logic2D::Logic2DTransformComponent& logic
		// );


		//
		//
		//
		// 入力状態をもとにIntentコンポーネントへ反映する
		static void UpdatePlayerMovementIntent(Engine::WorldSystem::Core::WorldCtx& ctx);

		// ワールド基準移動 (Global X-Z)
		//static void updatePlayerMovementIntent(
		//	Game::Character::Intent::MovementIntentComponent& intent
		//	, Game::Input::InputActionComponent& input
		//);


		// mouse input -> intent facing
		// static void updatePlayerDirectionIntent(
		// 	Engine::Component::AnalogInputComponent& analog,
		// 	Game::Character::Intent::FacingIntentComponent& facing,
		// 	Engine::Component::Logic2DTransformComponent& logic
		// );
	};
}