#pragma once

#include "Engine/ECS/Entity.h"

#include "Common/EngineNamespaceDecl.h"

namespace Engine::ECS::Component::Input
{
	struct InputBindingComponent
	{
		eNsECS::Entity controllingEntity;
	};
}