#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Layer
{
	class LocomotionAnimLayerFeature
	{
	public:
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);
	};

	//class LocomotionResolverLayerFeature
	//{
	//public:
	//	static void Update(Engine::ECS::EntityMgr& ecs);
	//};

	//class SkillAnimationResolverLayerFeature
	//{
	//public:
	//	static void Update(Engine::ECS::EntityMgr& ecs);
	//};

	//class CCAnimationResolverLayerFeature
	//{
	//public:
	//	static void Update(Engine::ECS::EntityMgr& ecs);
	//};

	//class AnimResolverLayerFeature
	//{
	//public:
	//	static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);
	//};
}