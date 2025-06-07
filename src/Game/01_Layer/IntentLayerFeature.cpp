#include "IntentLayerFeature.h"

#include "Game/00_Feature/Character/Movement/MovementFeature.h"

#include "Game/00_Feature/Combat/Skill/SkillFeature.h"

#include "Common/GameNamespaceDecl.h"

#include <iostream>

void Game::Layer::IntentLayerFeature::Update(eNsECS::EntityMgr& ecs)
{
	// Movement Intent
	gNsFeature::Character::MovementFeature::UpdateMovementIntent(ecs);
	// Intentに応じて各Intentコンポーネントのデータを更新
	gNsFeature::Character::MovementFeature::UpdateVelocity(ecs);

	// std::cout<<"hello\n";

	// Skill Intent
	gNsFeature::Combat::SkillFeature::UpateSkillIntent(ecs);

	// 以下Update Intent が続くはず
}
