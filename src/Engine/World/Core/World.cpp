#include "World.hpp"

#include "Engine/ECS/Core/Registry.hpp"
#include "Engine/World/Core/Resource.hpp"

namespace Engine::World::Core
{
	class World::Impl
	{
	public:
		Engine::ECS::Core::Registry ecs;
		Resource resources;
	};

	World::World() : p_(new Impl{}) {}
	World::~World() { delete p_; }

	Engine::ECS::Core::Entity World::Create()
	{
		return p_->ecs.Create();
	}

	void World::Destroy(Engine::ECS::Core::Entity e)
	{
		p_->ecs.Destroy(e);
	}

	bool World::IsAlive(Engine::ECS::Core::Entity e) const
	{
		return p_->ecs.IsAlive(e);
	}

	Engine::ECS::Core::Registry& World::Registry()
	{
		return p_->ecs;
	}

	const Engine::ECS::Core::Registry& World::Registry() const
	{
		return p_->ecs;
	}

	Resource& World::Resources() { return p_->resources; }
	const Resource& World::Resources() const { return p_->resources; }

}