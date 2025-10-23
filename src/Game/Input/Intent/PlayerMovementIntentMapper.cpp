#include "PlayerMovementIntentMapper.h"

#include "Game/Utils/SpatialTransformUtils.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"

#include "Game/ECS/Tags/CharacterAttribTags.h"

#include "Engine/Debug/Private/DebugUtils.h"

#include "Engine/Config/CanonicalDefaults.h"

#include <glm/glm.hpp>

// 最適化必要
// 最適化必要
// 最適化必要
// 最適化必要
// プレイヤーキャラクター用インテントコンポーネントと入力の連携 (Linking the Intent Component for Player Characters and Input)
void Game::Input::Intent::MovementIntentMappingSystem::UpdatePlayerMovementIntent(Engine::ECS::EntityMgr& ecs)
{
	// update player movement intent
	for (Engine::ECS::Entity e : ecs.view<
		Game::Input::InputActionComponent,
		Game::Character::Intent::MovementIntentComponent,
		Game::ECS::Tags::PlayerCharacterTag>())
	{
		auto& input = ecs.get<Game::Input::InputActionComponent>(e);
		auto& move = ecs.get<Game::Character::Intent::MovementIntentComponent>(e);

		updatePlayerMovementIntent(move, input);


	}

	// by mouse Cursor
	for (Engine::ECS::Entity e : ecs.view<
		Engine::ECS::Component::Input::AnalogInputComponent,
		Engine::ECS::Component::Input::InputBindingComponent>())
	{
		// バインドされたデータを取得
		auto& binding = ecs.get<Engine::ECS::Component::Input::InputBindingComponent>(e);

		// EntityおよびComponentが有効か確認
		if (!binding.controllingEntity.isValid()) continue;
		if (!ecs.isAlive(binding.controllingEntity)) continue;
		if (!ecs.hasComponent<Game::Character::Intent::FacingIntentComponent>(binding.controllingEntity)) continue;

		// アナログ入力データを取得
		auto& analog = ecs.get<Engine::ECS::Component::Input::AnalogInputComponent>(e);

		// 操作対象の`Entity`を取得
		auto& facing = ecs.get<Game::Character::Intent::FacingIntentComponent>(binding.controllingEntity);
		auto& logic = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(binding.controllingEntity);

		updatePlayerDirectionIntent(analog, facing, logic);
	}


	//for (Engine::ECS::Entity e : ecs.view <
	//	Engine::ECS::Component::Input::AnalogInputComponent
	//	, Game::Character::Intent::FacingIntentComponent
	//	>())
	//{
	//	auto& analog = ecs.get<Engine::ECS::Component::Input::AnalogInputComponent>(e);
	//	auto& logic = ecs.get <Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e);
	//	auto& facing = ecs.get<Game::Character::Intent::FacingIntentComponent>(e);

	//	updatePlayerDirectionIntent(facing, analog, logic);
	//}
}


void Game::Input::Intent::MovementIntentMappingSystem::updatePlayerMovementIntent(
	Game::Character::Intent::MovementIntentComponent& intent
	, Game::Input::InputActionComponent& input
)
{
	glm::vec2 moveDir(0.0f);
	if (input.isPressed(InputAction::MoveForward))	moveDir.y -= 1.0f;
	if (input.isPressed(InputAction::MoveBackward))	moveDir.y += 1.0f;
	if (input.isPressed(InputAction::MoveRight))	moveDir.x += 1.0f;
	if (input.isPressed(InputAction::MoveLeft))		moveDir.x -= 1.0f;

	//std::cout << "[MovementIntentMappingSystem] moveDir = ("
	//	<< moveDir.x << ", " << moveDir.y << ")\n";


	if (glm::length(moveDir) > 0.001f)
	{
		// directionは使用側で正規化する
		intent.direction = moveDir;
		intent.isActive = true;
	}
	else
	{
		intent.direction = glm::vec2(0.0f);
		intent.isActive = false;
	}
}

void Game::Input::Intent::MovementIntentMappingSystem::updatePlayerDirectionIntent(
	Engine::ECS::Component::Input::AnalogInputComponent& analog,
	Game::Character::Intent::FacingIntentComponent& facing,
	Engine::ECS::Component::Logic2D::Logic2DTransformComponent& logic
)
{

	glm::vec2 dir = analog.cursorLogicPositionXZ - logic.positionXZ;

	if (glm::length(dir) > 0.0001f && !glm::any(glm::isnan(dir)))
		facing.front = glm::normalize(dir);
	else
		facing.front = CanonicalDefaults::kCanonicalForwardXZ;


	// Engine::Debug::Logging::LogVector("PlayerIntentMapper.cpp(direction)", facing.front);
}