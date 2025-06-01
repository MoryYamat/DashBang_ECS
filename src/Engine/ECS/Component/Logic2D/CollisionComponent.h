// Collision Component 

#pragma once

#include "Engine/ECS/Component/Logic2D/ColliderType.h"

#include "Common/EngineNamespaceDecl.h"


namespace Engine::ECS::Component::Logic2D
{
	struct CollisionComponent
	{
		eNsLogic2DComp::Collider collider;
		bool isStatic = true;
	};
}
