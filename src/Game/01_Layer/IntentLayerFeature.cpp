#include "IntentLayerFeature.h"

#include "Game/00_Feature/MovementFeature.h"

#include "Common/GameNamespaceDecl.h"

void Game::Layer::IntentLayerFeature::Update(eNsECS::EntityMgr& ecs)
{
	// Movement Intent
	gNsFeature::MovementFeature::UpdateMovementIntent(ecs);
	// Intentに応じて各Intentコンポーネントのデータを更新
	gNsFeature::MovementFeature::UpdateVelocity(ecs);

	// Skill Intent など
}
