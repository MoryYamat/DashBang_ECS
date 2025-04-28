// (Child) entities have a hierarchical structure by having a parent entity ID.

#pragma once

#include "Core/ECS/Entity.h"

struct ParentComponent
{
	Entity parent;
};