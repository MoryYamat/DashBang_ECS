#include "World.hpp"



namespace Engine::World::Core
{
	Entity World::Create()
	{
		uint32_t id = nextId++;
		mAlive.insert(id);
		return Entity{ id };
	}

	void World::Destroy(Entity e)
	{
		if (!IsAlive(e)) return;
		for (auto& [_, pool] : mComponentPools) pool.erase(e.id);
		mAlive.erase(e.id);
	}

	bool World::IsAlive(Entity e) const 
	{
		return mAlive.count(e.id) != 0;
	}

}