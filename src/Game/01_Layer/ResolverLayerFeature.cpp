#include "ResolverLayerFeature.hpp"


#include "Game/00_Feature/Character/Control/Skill/CharacterSkillFeature.hpp"

#include "Game/00_Feature/Character/FSM/Movement/MovementFSMFeature.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Layer::ResolverLayerFeature::Update(eNsECS::EntityMgr& ecs)
{
	// Update Character Skill Intent Resolver
	gNsFeature::Character::CharacterSkillFeature::UpdateCharacterSkillIntentResolver(ecs);

	// Update Character Movement Intent Resolver
	// gNsFeature::Character::FSM::MovementFSMFeature::UpdateMovementIntentResolver(ecs);// íœ—\’èFFSM“±“üŒãíœ—\’è
}