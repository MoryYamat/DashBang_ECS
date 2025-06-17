#include "StateLayerFeature.hpp"

#include "Game/00_Feature/Character/State/CharacterStateFeature.hpp"


#include "Common/GameNamespaceDecl.h"

void Game::Layer::StateLayerFeature::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	// Update Character State
	gNsFeature::Character::StateFeature::UpdateCharacterState(ecs, deltaTime);
}