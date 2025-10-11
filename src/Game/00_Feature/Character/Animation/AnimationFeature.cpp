#include "AnimationFeature.hpp"

// locomotion
#include "Game/Character/Animation/Profile/Init/InitAnimationProfileDatabase.hpp"
#include "Game/Character/Animation/Query/Locomotion/BuildLocomotionAnimationQuerySystem.hpp"
#include "Game/Character/Animation/Resolve/Movement/MovementAnimationResolverSystem.hpp"
#include "Game/Character/Animation/Arbiter/AnimationArbiterSystem.hpp"
#include "Game/Character/Animation/Apply/ApplyFinalAnimationDecisionSystem.hpp"

// skill
#include "Game/Character/Animation/Profile/Init/InitSkillAnimProfileDatabase.hpp"
#include "Game/Character/Animation/Resolve/Skill/SkillAnimResolverSystem.hpp"
#include "Game/Character/Animation/Query/Skill/BuildSkillAnimQuerySystem.hpp"

namespace Game::Feature::Character::Animation
{
	namespace Anim = Game::Character::Animation;

	void CharacterAnimationFeature::InitProfileDatabase(Engine::ECS::EntityMgr& ecs)
	{
		Anim::Profile::Init::InitAnimationProfileDatabase(ecs);
	}

	void CharacterAnimationFeature::UpdateBuildAnimationQuery(Engine::ECS::EntityMgr& ecs)
	{
		Anim::Query::BuildLocomotionAnimationQuerySystem(ecs);
	}
	void CharacterAnimationFeature::UpdateMovementAnimationResolver(Engine::ECS::EntityMgr& ecs)
	{
		Anim::Resolve::Movement::MovementAnimationResolverSystem::Update(ecs);
	}
	void CharacterAnimationFeature::UpdateAnimationArbiterSystem(Engine::ECS::EntityMgr& ecs)
	{
		Anim::Arbiter::AnimationArbiterSystem::Update(ecs);
	}
	void CharacterAnimationFeature::UpdateApplyFinalAnimationDecision(Engine::ECS::EntityMgr& ecs)
	{
		Anim::Apply::ApplyFinalAnimationDecisionSystem::Update(ecs);
	}

}

// skill
namespace Game::Feature::Character::Animation
{
	namespace Anim = Game::Character::Animation;

	void CharacterAnimationFeature::InitSkillProfileDatabase(Engine::ECS::EntityMgr& ecs)
	{
		Anim::Profile::Init::InitSkillAnimProfileDatabase(ecs);
	}

	void CharacterAnimationFeature::UpdateSkillAnimationResolver(Engine::ECS::EntityMgr& ecs)
	{
		Anim::Resolve::Skill::SkillAnimResolverSystem::Update(ecs);
	}

	void CharacterAnimationFeature::UpdateBuildSkillAnimQuerySystem(Engine::ECS::EntityMgr& ecs)
	{
		Anim::Query::BuildSkillAnimQuerySystem(ecs);
	}
}