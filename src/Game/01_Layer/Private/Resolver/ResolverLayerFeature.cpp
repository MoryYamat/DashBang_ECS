#include "ResolverLayerFeature.hpp"


#include "Game/00_Feature/Private/Character/Control/Skill/CharacterSkillFeature.hpp"

#include "Game/00_Feature/Private/Character/FSM/Movement/MovementFSMFeature.hpp"

#include "Game/00_Feature/Private/Character/Control/CC/CharacterCCFeature.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

void Game::Layer::ResolverLayerFeature::Update(Engine::ECS::EntityMgr& ecs)
{
	// Update Character Skill Intent Resolver
	Game::Feature::Character::CharacterSkillFeature::UpdateCharacterSkillIntentResolver(ecs);

	// Update Character Movement Intent Resolver
	// Game::Feature::Character::FSM::MovementFSMFeature::UpdateMovementIntentResolver(ecs);// 削除予定：FSM導入後削除予定

	// update Character CC Antichain Component
	Game::Feature::Character::CharacterCCFeature::UpdateCCAntiChainSystem(ecs);
}

void Game::Layer::ResolverLayerFeature::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
{

}