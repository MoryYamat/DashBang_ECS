#include "CharacterFacingSystem.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Game/Character/Private/Control/Movement/Component/Intent/FacingIntentComponent.h"

#include "Game/Utils/Private/SpatialTransformUtils.h"

#include "Engine/Config/CanonicalDefaults.h"

#include "Engine/Debug/Private/DebugUtils.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <GLM/glm.hpp>

void Game::Character::Movement::UpdatePlayerFacing(Engine::ECS::EntityMgr& ecs)
{
	using namespace Engine::ECS::Component;

	namespace Ops = Engine::ECS::Ops;

	for (Engine::ECS::Entity e : ecs.view<
		Logic2D::Logic2DTransformComponent,
		Game::Character::Intent::FacingIntentComponent>())
	{
		auto& logic = Ops::Get<Logic2D::Logic2DTransformComponent>(ecs, e);
		auto& facing = Ops::Get<Game::Character::Intent::FacingIntentComponent>(ecs,e);

		logic.front = facing.front;
		logic.rotation = logic.GetRotationYFromFrontVector();
		logic.UpdateRightFromFront();

		// PlayerCharacterControllerは原則一つだけ
		// ecs.find<PlayerControllerComponent>()を用意してbreakを消す
		// ecs.find<PlayerControllerComponent>()を用意してbreakを消す
		// ecs.find<PlayerControllerComponent>()を用意してbreakを消す
		// ecs.find<PlayerControllerComponent>()を用意してbreakを消す
		break;
	}
}