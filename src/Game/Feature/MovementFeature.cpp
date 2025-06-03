#include "MovementFeature.h"

#include "Game/Character/Movement/System/CharacterMovementSystem.h"
#include "Game/Character/Movement/System/CharacterFacingSystem.h"

#include "Game/Character/Movement/System/UpdateCharacterVelocity.h"

#include "Common/GameNamespaceDecl.h"

void Game::Feature::MovementFeature::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	gNsCharacterMovement::UpdatePlayerFacing(ecs);
	gNsCharacterMovement::UpdateCharacterVelocity(ecs);
	gNsCharacterMovement::CharacterMovementSystem(ecs, deltaTime);
}