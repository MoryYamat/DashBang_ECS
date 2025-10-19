// MouseCursor Actor 

#pragma once 

#include "Engine/ECS/EntityManager.h"

#include "Game/Input/InputState.h"


namespace Game::Actor::AnalogInput
{
	class MouseCursorActor
	{
	public:
		MouseCursorActor(Engine::ECS::EntityMgr& ecs);
	};
}
