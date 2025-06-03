#include "CharacterFacingSystem.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Game/Character/Movement/Component/Intent/FacingIntentComponent.h"

#include "Game/Utils/SpatialTransformUtils.h"

#include "Engine/Config/CanonicalDefaults.h"

#include "Engine/Debug/DebugUtils.h"

#include <GLM/glm.hpp>

void Game::Character::Movement::UpdatePlayerFacing(eNsECS::EntityMgr& ecs)
{
	for (eNsECS::Entity e : ecs.view<
		eNsLogic2DComp::Logic2DTransformComponent,
		gNsCharacterIntent::FacingIntentComponent>())
	{
		auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(e);
		auto& facing = ecs.get<gNsCharacterIntent::FacingIntentComponent>(e);

		logic.front = facing.front;
		logic.UpdateRightFromFront();

		// PlayerCharacterControllerは原則一つだけ
		// ecs.find<PlayerControllerComponent>()を用意してbreakを消す
		// ecs.find<PlayerControllerComponent>()を用意してbreakを消す
		// ecs.find<PlayerControllerComponent>()を用意してbreakを消す
		// ecs.find<PlayerControllerComponent>()を用意してbreakを消す
		break;
	}
}