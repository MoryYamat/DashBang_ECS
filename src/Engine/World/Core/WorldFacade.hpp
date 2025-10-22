#pragma once

#include "Engine/World/Core/World.hpp"
#include "Engine/World/Core/WorldAccess.hpp"

namespace Engine::World::Core
{
	class WorldFacade
	{
	public:

		explicit WorldFacade(World& w) : world_(w){}

		World& Get() { return world_; }
		const World& Get() const { return world_; }

		WorldRead Read() const { return { world_ }; }
		WorldWrite Write() { return { world_ }; }

	private:
		World& world_;
	};
}