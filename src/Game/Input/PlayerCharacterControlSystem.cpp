#include "PlayerCharacterControlSystem.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/PlayerControllerComponent.h"


void PlayerCharacterControlSystem::Update(ECS& ecs, InputState& input, float deltaTime)
{
	for (Entity e : ecs.view<PlayerControllerComponent, TransformComponent>())
	{

	}
}
