#include "SkillFSMFeature.hpp"

#include "Game/Combat/Skill/FSM/System/SkillFSMSystem.hpp"
#include "Game/Combat/Skill/FSM/System/SkillFSMResolverSystem.hpp"
// skill interference
#include "Game/Combat/Skill/FSM/Interference/SkillFSMInterferenceResolver.hpp"


namespace Game::Feature::Character::Skill::FSM
{
	void SkillFSMFeature::UpdateSkillFSMSystem(Engine::ECS::EntityMgr& ecs, float deltaTime)
	{
		// (内部起因の)transition request 送信
		Game::Combat::Skill::FSM::UpdateSkillFSMSystem(ecs, deltaTime);

	}

	void SkillFSMFeature::UpdateSkillFSMResolverSystem(Engine::ECS::EntityMgr& ecs, float deltaTime)
	{
		Game::Combat::Skill::FSM::System::SkillFSMResolverSystem::Update(ecs, deltaTime);
	}

	void SkillFSMFeature::UpdateSkillInterferenceResolver(Engine::ECS::EntityMgr& ecs)
	{
		Game::Combat::Skill::FSM::Interference::SkillFSMInterferenceResolver::Update(ecs);
	}
}

