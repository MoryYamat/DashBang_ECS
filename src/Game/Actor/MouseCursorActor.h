// MouseCursor Actor 

#pragma once 

#include "Engine/ECS/EntityManager.h"

#include "Game/Input/InputState.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Actor::AnalogInput
{
	class MouseCursorActor
	{
	public:
		MouseCursorActor(eNsECS::EntityMgr& ecs);
	};
}
