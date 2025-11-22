#pragma once

//
#include "Engine/ECS/EntityManager.h"

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Engine::Graphics::Animation::System
{
	void AnimationSystem(Engine::ECS::EntityMgr& ecs);

	void AnimationSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
}