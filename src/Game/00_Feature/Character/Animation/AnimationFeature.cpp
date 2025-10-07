#include "AnimationFeature.hpp"

#include "Game/Character/Animation/Profile/Init/InitAnimationProfileDatabase.hpp"
#include "Game/Character/Animation/Query/BuildAnimationQuerySystem.hpp"
#include "Game/Character/Animation/Resolve/Movement/MovementAnimationResolverSystem.hpp"
#include "Game/Character/Animation/Arbiter/AnimationArbiterSystem.hpp"
#include "Game/Character/Animation/Apply/ApplyFinalAnimationDecisionSystem.hpp"



namespace Game::Feature::Character::Animation
{
	namespace Anim = Game::Character::Animation;

	void CharacterAnimationFeature::InitProfileDatabase(Engine::ECS::EntityMgr& ecs)
	{
		Anim::Profile::Init::InitAnimationProfileDatabase(ecs);
	}

	void CharacterAnimationFeature::UpdateBuildAnimationQuery(Engine::ECS::EntityMgr& ecs)
	{
		Anim::Query::BuildAnimationQuerySystem(ecs);
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