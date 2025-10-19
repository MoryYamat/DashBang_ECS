#include "IntentLayerFeature.h"

#include "Game/00_Feature/Character/Movement/MovementFeature.h"

#include "Game/00_Feature/Combat/Skill/SkillFeature.h"

#include <iostream>

void Game::Layer::IntentLayerFeature::Update(Engine::ECS::EntityMgr& ecs)
{
	// Movement Intent
	Game::Feature::Character::MovementFeature::UpdateMovementIntent(ecs);
	// Intentに応じて各Intentコンポーネントのデータを更新
	// Game::Feature::Character::MovementFeature::UpdateVelocity(ecs);

	// std::cout<<"hello\n";

	// Skill Intent
	Game::Feature::Combat::SkillFeature::UpateSkillIntent(ecs);

	// 以下Update Intent が続くはず
}
