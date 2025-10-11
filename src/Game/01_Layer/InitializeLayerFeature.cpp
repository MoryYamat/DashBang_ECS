#include "InitializeLayerFeature.h"


#include "Game/00_Feature/Combat/Skill/SkillFeature.h"

#include "Game/00_Feature/System/InputFeature.h"

#include "Game/00_Feature/Collision/CollisionFeature.h"

#include "Game/00_Feature/Character/Animation/AnimationFeature.hpp"

#include "Game/00_Feature/Character/State/CharacterStateFeature.hpp"

#include "Game/00_Feature/Character/FSM/Movement/MovementFSMFeature.hpp"

#include "Game/00_Feature/Character/FSM/CC/CCFSMFeature.hpp"

#include "Game/00_Feature/System/WorldClockFeature.hpp"

#include "Game/00_Feature/Combat/HitEvent/HitEventFeature.hpp"

// control
#include "Game/00_Feature/Character/Control/CC/CharacterCCFeature.hpp"

#include "Common/GameNamespaceDecl.h"

void Game::Layer::InitializeLayerFeature::DelayedInitialzation(eNsECS::EntityMgr& ecs)
{
	// --------------------- 初期化順に注意 --------------------- 
	
	gNsFeature::System::WorldClockFeature::Init(ecs);

	// InputAction(抽象入力)の割り当て初期化
	gNsFeature::Setting::InputFeature::InitInputMapping(ecs);

	// スキルスロット(gNsSkillData::SkillSlot)とInputAction(抽象入力)の割り当て初期化
	gNsFeature::Setting::InputFeature::InitSkillInputMapping(ecs);

	// スキルデータベース/定義初期化
	gNsFeature::Combat::SkillFeature::InitializeSkillDatabase(ecs);

	gNsFeature::Character::FSM::CCFSMFeature::InitCCFSMDefinitionDatabase(ecs);

	// HitEvent
	gNsFeature::Combat::HitEventFeature::InitializeHitEventDatabase(ecs);

	// Collision Result Buffer
	gNsFeature::CollisionFeature::InitCollisionBuffer(ecs);

	// CharacterStateTransitionデータベース初期化
	gNsFeature::Character::StateFeature::InitTransitionDatabase(ecs);

	// MovementFSMの定義を初期化
	gNsFeature::Character::FSM::MovementFSMFeature::InitializeMovementFSMDefinition(ecs);


	// AntichainPolicy初期化
	gNsFeature::Character::CharacterCCFeature::InitCCAntiChainPolicy(ecs);

	// animation
	Game::Feature::Character::Animation::CharacterAnimationFeature::InitProfileDatabase(ecs);
	Game::Feature::Character::Animation::CharacterAnimationFeature::InitSkillProfileDatabase(ecs);
}