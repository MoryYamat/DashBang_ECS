#pragma once

#include "World.hpp"
#include "WorldFacade.hpp"

namespace Engine::World::Core
{
	struct WorldCtx
	{
		Engine::World::Core::WorldRead rw;
		Engine::World::Core::WorldWrite ww;

		explicit WorldCtx(Engine::World::Core::World& w) : rw{w}, ww{ w } {}
	};
}