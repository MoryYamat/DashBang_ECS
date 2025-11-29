#include "IntentLayerFeature.h"

#include "Game/00_Feature/Private/Character/Movement/MovementFeature.h"
#include "Game/00_Feature/Private/Character/Control/Skill/CharacterSkillFeature.hpp"

#include "Game/00_Feature/Private/Combat/Skill/SkillFeature.h"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include <iostream>

void Game::Layer::IntentLayerFeature::Update(Engine::ECS::EntityMgr& ecs)
{
	// Movement Intent
	//Game::Feature::Character::MovementFeature::UpdateMovementIntent(ecs);
	// Intentに応じて各Intentコンポーネントのデータを更新
	// Game::Feature::Character::MovementFeature::UpdateVelocity(ecs);

	// std::cout<<"hello\n";

	//// Skill Intent
	//Game::Feature::Combat::SkillFeature::UpateSkillIntent(ecs);

	// 以下Update Intent が続くはず
}

void Game::Layer::IntentLayerFeature::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	Game::Feature::Character::MovementFeature::UpdateMovementIntent(ctx);
	Game::Feature::Character::MovementFeature::UpdateLogicFacing(ctx);

	Game::Feature::Character::CharacterSkillFeature::UpdateCharacterSkillIntent(ctx);
}