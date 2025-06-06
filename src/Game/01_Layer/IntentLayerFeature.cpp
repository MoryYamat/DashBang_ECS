#include "IntentLayerFeature.h"

#include "Game/00_Feature/Movement/MovementFeature.h"

#include "Game/00_Feature/Skill/SkillFeature.h"

#include "Common/GameNamespaceDecl.h"

void Game::Layer::IntentLayerFeature::Update(eNsECS::EntityMgr& ecs)
{
	// Movement Intent
	gNsFeature::MovementFeature::UpdateMovementIntent(ecs);
	// Intentに応じて各Intentコンポーネントのデータを更新
	gNsFeature::MovementFeature::UpdateVelocity(ecs);

	// Skill Intent
	gNsFeature::SkillFeature::UpateSkillIntent(ecs);

	// 以下Update Intent が続くはず
}
