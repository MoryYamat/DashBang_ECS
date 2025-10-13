#include "LocomotionResolverLayerFeature.hpp"

#include "Game/00_Feature/Character/Animation/AnimationFeature.hpp"

namespace Game::Layer
{
	namespace Anim = Game::Feature::Character::Animation;

	void LocomotionResolverLayerFeature::Update(Engine::ECS::EntityMgr& ecs)
	{
		Anim::CharacterAnimationFeature::UpdateBuildAnimationQuery(ecs);

		Anim::CharacterAnimationFeature::UpdateMovementAnimationResolver(ecs);

		Anim::CharacterAnimationFeature::UpdateAnimationArbiterSystem(ecs);

		Anim::CharacterAnimationFeature::UpdateApplyFinalAnimationDecision(ecs);
	}

	void SkillAnimationResolverLayerFeature::Update(Engine::ECS::EntityMgr& ecs)
	{
		Anim::CharacterAnimationFeature::UpdateBuildSkillAnimQuerySystem(ecs);

		Anim::CharacterAnimationFeature::UpdateSkillAnimationResolver(ecs);
	}

	void CCAnimationResolverLayerFeature::Update(Engine::ECS::EntityMgr& ecs)
	{
		Anim::CharacterAnimationFeature::UpdateBuildCCAnimQuerySystem(ecs);
		Anim::CharacterAnimationFeature::UpdateCCAnimationResolver(ecs);
	}
}