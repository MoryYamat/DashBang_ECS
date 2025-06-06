#include "MovementFeature.h"

#include "Game/Character/Movement/System/CharacterMovementSystem.h"
#include "Game/Character/Movement/System/CharacterFacingSystem.h"

#include "Game/Character/Movement/System/UpdateCharacterVelocity.h"

#include "Game/Input/Intent/PlayerMovementIntentMapper.h"

#include "Common/GameNamespaceDecl.h"

// Intent
void Game::Feature::MovementFeature::UpdateMovementIntent(eNsECS::EntityMgr& ecs)
{
	gNsInputIntent::MovementIntentMappingSystem::UpdatePlayerMovementIntent(ecs);
}

// é¿çsï\åª (Action expression)
void Game::Feature::MovementFeature::UpdateVelocity(eNsECS::EntityMgr& ecs)
{
	gNsCharacterMovement::UpdateCharacterVelocity(ecs);
}


// logic
void Game::Feature::MovementFeature::UpdateLogicFacing(eNsECS::EntityMgr& ecs)
{
	gNsCharacterMovement::UpdatePlayerFacing(ecs);
}

// logic
void Game::Feature::MovementFeature::UpdateLogicPosition(eNsECS::EntityMgr& ecs, float deltaTime)
{
	gNsCharacterMovement::CharacterMovementSystem(ecs, deltaTime);
}