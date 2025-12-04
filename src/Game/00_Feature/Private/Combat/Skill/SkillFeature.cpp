#include "SkillFeature.h"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"


#include "Game/Combat/Skill/Core/Public/SkillApi.hpp"
#include "Game/Combat/Skill/Binding/Public/API.hpp"
#include "Game/Combat/Skill/Runtime/Public/SkillRuntimeAPI.hpp"

//
//
namespace Game::Feature::Combat
{
	void SkillFeature::InitSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::Skill::InitAllSkillSystem(ctx);
	}

	void SkillFeature::UpdateSkillRuntimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::Skill::Runtime::UpdateAllSkillRuntimeSystem(ctx);
	}

	void SkillFeature::InitSkillBindingSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::Skill::Binding::InitAllSkillBindingSystem(ctx);
	}
}