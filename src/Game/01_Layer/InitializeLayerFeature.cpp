#include "InitializeLayerFeature.h"

#include "Game/00_Feature/Combat/Skill/SkillFeature.h"

#include "Game/00_Feature/System/InputFeature.h"

#include "Game/00_Feature/Collision/CollisionFeature.h"

#include "Game/00_Feature/Character/State/CharacterStateFeature.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Layer::InitializeLayerFeature::DelayedInitialzation(eNsECS::EntityMgr& ecs)
{
	// --------------------- 初期化順に注意 --------------------- 
	
	// InputAction(抽象入力)の割り当て初期化
	gNsFeature::Setting::InputFeature::InitInputMapping(ecs);

	// スキルスロット(gNsSkillData::SkillSlot)とInputAction(抽象入力)の割り当て初期化
	gNsFeature::Setting::InputFeature::InitSkillInputMapping(ecs);

	// スキルデータベース/定義初期化
	gNsFeature::Combat::SkillFeature::InitializeSkillDatabase(ecs);

	// Collision Result Buffer
	gNsFeature::CollisionFeature::InitCollisionBuffer(ecs);

	// CharacterStateTransitionデータベース初期化
	gNsFeature::Character::StateFeature::InitTransitionDatabase(ecs);
}