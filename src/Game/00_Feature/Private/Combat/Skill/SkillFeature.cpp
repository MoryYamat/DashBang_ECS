#include "SkillFeature.h"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"


#include "Game/Combat/Skill/Public/SkillApi.hpp"

//
//
namespace Game::Feature::Combat
{
	void SkillFeature::InitSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::Skill::InitAllSkillSystem(ctx);
	}

	void SkillFeature::UpdateSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::Skill::UpdateAllSkillRuntimeSystem(ctx);
	}
}