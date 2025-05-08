// Collision Component 

#pragma once

#include "Core/ECS/Component/Collision/ColliderType.h"

struct CollisionComponent
{
	Collider collider;
	bool isStatic = true;
};