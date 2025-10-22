#pragma once

#include "Engine/World/Core/World.hpp"
#include "Engine/World/Core/WorldAccess.hpp"

namespace Engine::World::Core
{
	struct WorldCtx
	{
		World& world;
		WorldRead rw{ world };
		WorldWrite ww{ world };
	};
}