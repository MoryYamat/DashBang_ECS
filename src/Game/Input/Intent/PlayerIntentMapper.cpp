#include "PlayerIntentMapper.h"

#include "Game/Utils/SpatialTransformUtils.h"


// Update Character Intent Components
void Game::Input::Intent::IntentMappingSystem::UpdatePlayerIntent(eNsECS::EntityMgr& ecs
	, const eNsInput::RawInputState& rawInput
	, const eNsGfxRender::RenderContext& renderContext
)
{

	for (eNsECS::Entity e : ecs.view<gNsInput::InputActionComponent,
		gNsCharacterIntent::MovementIntentComponent
		, gNsCharacterIntent::FacingIntentComponent
		, eNsLogic2DComp::Logic2DTransformComponent>()
		)
	{
		auto& input = ecs.get<gNsInput::InputActionComponent>(e);
		auto& move = ecs.get<gNsCharacterIntent::MovementIntentComponent>(e);
		auto& facing = ecs.get<gNsCharacterIntent::FacingIntentComponent>(e);
		auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(e);

		updatePlayerMovementIntent(move, input);
	}
}


void Game::Input::Intent::IntentMappingSystem::updatePlayerMovementIntent(gNsCharacterIntent::MovementIntentComponent& intent, gNsInput::InputActionComponent& input)
{
	glm::vec2 moveDir(0.0f);
	if (input.isPressed(InputAction::MoveForward))	moveDir.y -= 1.0f;
	if (input.isPressed(InputAction::MoveBackward))	moveDir.y += 1.0f;
	if (input.isPressed(InputAction::MoveRight))	moveDir.x += 1.0f;
	if (input.isPressed(InputAction::MoveLeft))		moveDir.x -= 1.0f;

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


void Game::Input::Intent::IntentMappingSystem::updatePlayerDirectionIntent(gNsCharacterIntent::FacingIntentComponent& intent, eNsInputComp::AnalogInputComponent& analogInput)
{

}