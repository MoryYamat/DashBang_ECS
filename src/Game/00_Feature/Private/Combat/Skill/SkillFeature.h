// 

#pragma once

#include "Engine/ECS/EntityManager.h"



#include "Engine/WorldSystem/Public/WorldFwd.hpp"



namespace Game::Feature::Combat
{
	class SkillFeature
	{
	public:
		static void InitSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
		static void UpdateSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}