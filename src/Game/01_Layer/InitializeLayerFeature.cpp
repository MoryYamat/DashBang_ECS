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



void Game::Layer::InitializeLayerFeature::DelayedInitialzation(Engine::ECS::EntityMgr& ecs)
{
	// --------------------- 初期化順に注意 --------------------- 
	
	Game::Feature::System::WorldClockFeature::Init(ecs);

	// InputAction(抽象入力)の割り当て初期化
	Game::Feature::Setting::InputFeature::InitInputMapping(ecs);

	// スキルスロット(Game::Combat::Skill::Data::SkillSlot)とInputAction(抽象入力)の割り当て初期化
	Game::Feature::Setting::InputFeature::InitSkillInputMapping(ecs);

	// スキルデータベース/定義初期化
	Game::Feature::Combat::SkillFeature::InitializeSkillDatabase(ecs);

	Game::Feature::Character::FSM::CCFSMFeature::InitCCFSMDefinitionDatabase(ecs);

	// HitEvent
	Game::Feature::Combat::HitEventFeature::InitializeHitEventDatabase(ecs);

	// Collision Result Buffer
	Game::Feature::CollisionFeature::InitCollisionBuffer(ecs);

	// CharacterStateTransitionデータベース初期化
	Game::Feature::Character::StateFeature::InitTransitionDatabase(ecs);

	// MovementFSMの定義を初期化
	Game::Feature::Character::FSM::MovementFSMFeature::InitializeMovementFSMDefinition(ecs);


	// AntichainPolicy初期化
	Game::Feature::Character::CharacterCCFeature::InitCCAntiChainPolicy(ecs);

	// animation
	Game::Feature::Character::Animation::CharacterAnimationFeature::InitProfileDatabase(ecs);// locomotion
	Game::Feature::Character::Animation::CharacterAnimationFeature::InitSkillProfileDatabase(ecs);// skill
	Game::Feature::Character::Animation::CharacterAnimationFeature::InitCCProfileDatabase(ecs);// cc
}