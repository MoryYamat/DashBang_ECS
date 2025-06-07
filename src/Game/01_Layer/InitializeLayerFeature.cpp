#include "InitializeLayerFeature.h"

#include "Game/00_Feature/Combat/Skill/SkillFeature.h"

#include "Game/00_Feature/System/InputFeature.h"

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
}