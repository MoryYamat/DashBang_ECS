#include "PlayerMovementIntentMapper.h"

#include "Game/Utils/SpatialTransformUtils.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"

#include "Engine/Debug/DebugUtils.h"

#include "Engine/Config/CanonicalDefaults.h"

#include <glm/glm.hpp>

// 最適化必要
// 最適化必要
// 最適化必要
// 最適化必要
// プレイヤーキャラクター用インテントコンポーネントと入力の連携 (Linking the Intent Component for Player Characters and Input)
void Game::Input::Intent::MovementIntentMappingSystem::UpdatePlayerMovementIntent(eNsECS::EntityMgr& ecs)
{
	// update player movement intent
	for (eNsECS::Entity e : ecs.view<
		gNsInput::InputActionComponent
		, gNsCharacterIntent::MovementIntentComponent
		, eNsTagComp::PlayerControllerComponent
	>())
	{
		auto& input = ecs.get<gNsInput::InputActionComponent>(e);
		auto& move = ecs.get<gNsCharacterIntent::MovementIntentComponent>(e);

		updatePlayerMovementIntent(move, input);


	}

	// by mouse Cursor
	for (eNsECS::Entity e : ecs.view<
		eNsInputComp::AnalogInputComponent,
		eNsInputComp::InputBindingComponent>())
	{
		// バインドされたデータを取得
		auto& binding = ecs.get<eNsInputComp::InputBindingComponent>(e);

		// EntityおよびComponentが有効か確認
		if (!binding.controllingEntity.isValid()) continue;
		if (!ecs.isAlive(binding.controllingEntity)) continue;
		if (!ecs.hasComponent<gNsCharacterIntent::FacingIntentComponent>(binding.controllingEntity)) continue;

		// アナログ入力データを取得
		auto& analog = ecs.get<eNsInputComp::AnalogInputComponent>(e);

		// 操作対象の`Entity`を取得
		auto& facing = ecs.get<gNsCharacterIntent::FacingIntentComponent>(binding.controllingEntity);
		auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(binding.controllingEntity);

		updatePlayerDirectionIntent(analog, facing, logic);
	}


	//for (eNsECS::Entity e : ecs.view <
	//	eNsInputComp::AnalogInputComponent
	//	, gNsCharacterIntent::FacingIntentComponent
	//	>())
	//{
	//	auto& analog = ecs.get<eNsInputComp::AnalogInputComponent>(e);
	//	auto& logic = ecs.get <eNsLogic2DComp::Logic2DTransformComponent>(e);
	//	auto& facing = ecs.get<gNsCharacterIntent::FacingIntentComponent>(e);

	//	updatePlayerDirectionIntent(facing, analog, logic);
	//}
}


void Game::Input::Intent::MovementIntentMappingSystem::updatePlayerMovementIntent(
	gNsCharacterIntent::MovementIntentComponent& intent
	, gNsInput::InputActionComponent& input
)
{
	glm::vec2 moveDir(0.0f);
	if (input.isPressed(InputAction::MoveForward))	moveDir.y -= 1.0f;
	if (input.isPressed(InputAction::MoveBackward))	moveDir.y += 1.0f;
	if (input.isPressed(InputAction::MoveRight))	moveDir.x += 1.0f;
	if (input.isPressed(InputAction::MoveLeft))		moveDir.x -= 1.0f;


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
	eNsInputComp::AnalogInputComponent& analog,
	gNsCharacterIntent::FacingIntentComponent& facing,
	eNsLogic2DComp::Logic2DTransformComponent& logic
)
{

	glm::vec2 dir = analog.cursorLogicPositionXZ - logic.positionXZ;

	if (glm::length(dir) > 0.0001f && !glm::any(glm::isnan(dir)))
		facing.front = glm::normalize(dir);
	else
		facing.front = CanonicalDefaults::kCanonicalFrowardXZ;


	// eNsDebugLog::LogVector("PlayerIntentMapper.cpp(direction)", facing.front);
}