#include "PlayerMovementIntentMapper.h"

#include "Game/Utils/Private/SpatialTransformUtils.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"


#include "Engine/Debug/Private/DebugUtils.h"

#include "Engine/Config/CanonicalDefaults.h"



//
#include "Engine/Component/Private/Input/InputBindingComponent.hpp"
#include "Engine/Component/Private/Input/AnalogInputComponent.hpp"
#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Game/Input/Private/InputActionComponent.h"
#include "Game/Character/Private/Control/Public/IntentComponent.hpp"
#include "Game/ECS/Public/CharacterAttribTags.h"
//
#include <glm/glm.hpp>

// 最適化必要
// 最適化必要
// 最適化必要
// 最適化必要
// プレイヤーキャラクター用インテントコンポーネントと入力の連携 (Linking the Intent Component for Player Characters and Input)
//void Game::Input::Intent::MovementIntentMappingSystem::UpdatePlayerMovementIntent(Engine::ECS::EntityMgr& ecs)
//{
//	// update player movement intent
//	for (Engine::ECS::Entity e : ecs.view<
//		Game::Input::InputActionComponent,
//		Game::Character::Intent::MovementIntentComponent,
//		Game::ECS::Tags::PlayerCharacterTag>())
//	{
//		auto& input = ecs.get<Game::Input::InputActionComponent>(e);
//		auto& move = ecs.get<Game::Character::Intent::MovementIntentComponent>(e);
//
//		updatePlayerMovementIntent(move, input);
//
//
//	}
//
//	// by mouse Cursor
//	for (Engine::ECS::Entity e : ecs.view<
//		Engine::ECS::Component::Input::AnalogInputComponent,
//		Engine::ECS::Component::Input::InputBindingComponent>())
//	{
//		// バインドされたデータを取得
//		auto& binding = ecs.get<Engine::ECS::Component::Input::InputBindingComponent>(e);
//
//		// EntityおよびComponentが有効か確認
//		if (!binding.controllingEntity.isValid()) continue;
//		if (!ecs.isAlive(binding.controllingEntity)) continue;
//		if (!ecs.hasComponent<Game::Character::Intent::FacingIntentComponent>(binding.controllingEntity)) continue;
//
//		// アナログ入力データを取得
//		auto& analog = ecs.get<Engine::ECS::Component::Input::AnalogInputComponent>(e);
//
//		// 操作対象の`Entity`を取得
//		auto& facing = ecs.get<Game::Character::Intent::FacingIntentComponent>(binding.controllingEntity);
//		auto& logic = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(binding.controllingEntity);
//
//		updatePlayerDirectionIntent(analog, facing, logic);
//	}
//
//
//	//for (Engine::ECS::Entity e : ecs.view <
//	//	Engine::ECS::Component::Input::AnalogInputComponent
//	//	, Game::Character::Intent::FacingIntentComponent
//	//	>())
//	//{
//	//	auto& analog = ecs.get<Engine::ECS::Component::Input::AnalogInputComponent>(e);
//	//	auto& logic = ecs.get <Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e);
//	//	auto& facing = ecs.get<Game::Character::Intent::FacingIntentComponent>(e);
//
//	//	updatePlayerDirectionIntent(facing, analog, logic);
//	//}
//}
//
//
//void Game::Input::Intent::MovementIntentMappingSystem::updatePlayerMovementIntent(
//	Game::Character::Intent::MovementIntentComponent& intent
//	, Game::Input::InputActionComponent& input
//)
//{
//	glm::vec2 moveDir(0.0f);
//	if (input.isPressed(InputAction::MoveForward))	moveDir.y -= 1.0f;
//	if (input.isPressed(InputAction::MoveBackward))	moveDir.y += 1.0f;
//	if (input.isPressed(InputAction::MoveRight))	moveDir.x += 1.0f;
//	if (input.isPressed(InputAction::MoveLeft))		moveDir.x -= 1.0f;
//
//	//std::cout << "[MovementIntentMappingSystem] moveDir = ("
//	//	<< moveDir.x << ", " << moveDir.y << ")\n";
//
//
//	if (glm::length(moveDir) > 0.001f)
//	{
//		// directionは使用側で正規化する
//		intent.direction = moveDir;
//		intent.isActive = true;
//	}
//	else
//	{
//		intent.direction = glm::vec2(0.0f);
//		intent.isActive = false;
//	}
//}
//
//void Game::Input::Intent::MovementIntentMappingSystem::updatePlayerDirectionIntent(
//	Engine::ECS::Component::Input::AnalogInputComponent& analog,
//	Game::Character::Intent::FacingIntentComponent& facing,
//	Engine::ECS::Component::Logic2D::Logic2DTransformComponent& logic
//)
//{
//
//	glm::vec2 dir = analog.cursorLogicPositionXZ - logic.positionXZ;
//
//	if (glm::length(dir) > 0.0001f && !glm::any(glm::isnan(dir)))
//		facing.front = glm::normalize(dir);
//	else
//		facing.front = CanonicalDefaults::kCanonicalForwardXZ;
//
//
//	// Engine::Debug::Logging::LogVector("PlayerIntentMapper.cpp(direction)", facing.front);
//}

namespace Game::Input::Intent
{
	using namespace Engine::WorldSystem;
	using namespace Game::Character::Control;

	static void updatePlayerMovementIntent(
		Game::Character::Control::MovingIntentComponent& intent, 
		Game::Input::InputActionComponent& input)
	{
		glm::vec2 moveDir(0.0f);
		if (input.isPressed(InputAction::MoveForward))	moveDir.y -= 1.0f;
		if (input.isPressed(InputAction::MoveBackward))	moveDir.y += 1.0f;
		if (input.isPressed(InputAction::MoveRight))	moveDir.x += 1.0f;
		if (input.isPressed(InputAction::MoveLeft))		moveDir.x -= 1.0f;

		// std::cout << "[MovementIntentMappingSystem] moveDir = ("
		// 	<< moveDir.x << ", " << moveDir.y << ")\n";


		if (glm::length(moveDir) > 0.001f)
		{
			intent.direction = glm::normalize(moveDir);
			intent.isActive = true;
		}
		else
		{
			intent.direction = glm::vec2(0.0f);
			intent.isActive = false;
		}
		
		

	}

	static void updatePlayerDirectionIntent(
		Engine::Component::AnalogInputComponent& analog,
		Game::Character::Control::FacingIntentComponent& facing,
		Engine::Component::Logic2DTransformComponent& logicOfControllingEntity
	)
	{
		glm::vec2 dir = analog.cursorLogicPositionXZ - logicOfControllingEntity.positionXZ;

		if (glm::length(dir) > 0.0001f && !glm::any(glm::isnan(dir)))
			facing.front = glm::normalize(dir);
		else
			facing.front = CanonicalDefaults::kCanonicalForwardXZ;


		// Engine::Debug::Logging::LogVector("PlayerIntentMapper.cpp(direction)", facing.front);
	}

	void MovementIntentMappingSystem::UpdatePlayerMovementIntent(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto ents = Query::ViewWhere(ctx.rw, Query::All<Game::Character::Control::MovingIntentComponent,
			Game::Input::InputActionComponent, Game::ECS::Tags::PlayerCharacterTag>{});

		for (const auto& e : ents)
		{
			auto& input = ctx.ww.Get<InputActionComponent>(e);
			auto& intent = ctx.ww.Get<Game::Character::Control::MovingIntentComponent>(e);

			updatePlayerMovementIntent(intent, input);
		}

		auto ent = Query::ViewWhere(ctx.rw, Query::All<
			Engine::Component::AnalogInputComponent,
			Engine::Component::InputBindingComponent>{});

		for (const auto& e : ent)
		{
			auto& analog = ctx.ww.Get<Engine::Component::AnalogInputComponent>(e);
			auto& binding = ctx.ww.Get<Engine::Component::InputBindingComponent>(e);

			auto& facing = ctx.ww.Get<Game::Character::Control::FacingIntentComponent>(binding.controllingEntity);
			auto& playerLogic = ctx.ww.Get<Engine::Component::Logic2DTransformComponent>(binding.controllingEntity);

			updatePlayerDirectionIntent(analog, facing, playerLogic);
		}
	}
}