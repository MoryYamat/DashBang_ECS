#include "ResolverLayerFeature.hpp"


#include "Game/00_Feature/Character/Control/Skill/CharacterSkillFeature.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Layer::ResolverLayerFeature::Update(eNsECS::EntityMgr& ecs)
{
	// Update Character Skill Intent Resolver
	gNsFeature::Character::CharacterSkillFeature::UpdateCharacterSkillIntentResolver(ecs);
}