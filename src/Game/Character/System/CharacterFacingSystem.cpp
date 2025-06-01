#include "CharacterFacingSystem.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Game/Utils/SpatialTransformUtils.h"

#include "Engine/Config/CanonicalDefaults.h"

#include "Engine/Debug/DebugUtils.h"

#include <GLM/glm.hpp>

void Game::Character::Facing::UpdatePlayerFacing(eNsECS::EntityMgr& ecs, const eNsInput::RawInputState& rawInput, const eNsGfxRender::RenderContext& renderContext)
{
	for (eNsECS::Entity e : ecs.view<eNsTagComp::PlayerControllerComponent, eNsLogic2DComp::Logic2DTransformComponent>())
	{
		auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(e);



		// PlayerCharacterControllerは原則一つだけ
		// ecs.find<PlayerControllerComponent>()を用意してbreakを消す
		// ecs.find<PlayerControllerComponent>()を用意してbreakを消す
		// ecs.find<PlayerControllerComponent>()を用意してbreakを消す
		// ecs.find<PlayerControllerComponent>()を用意してbreakを消す
		break;
	}
}