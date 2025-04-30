// Collision Component 

#pragma once

#include "DataTypes/ColliderType.h"

struct CollisionComponent
{
	Collider collider;
	bool isStatic = true;
};