#include "CharacterSkillFeature.hpp"

#include "Game/Character/Control/Skill/System/CharacterSkillExecutionSystem.hpp"

#include "Game/Character/Control/Skill/System/CharacterSkillIntentResolver.hpp"

#include "Common/GameNamespaceDecl.h"

// キャラクターのスキル実行を更新する
void Game::Feature::Character::CharacterSkillFeature::UpdateCharacterSkillExecution(eNsECS::EntityMgr& ecs, float deltaTime)
{
	gNsCharacterControlSkill::UpdateCharacterSkillExecution(ecs, deltaTime);
}

// キャラクターのスキル意図を反映可能かどうか判定し，更新する
void Game::Feature::Character::CharacterSkillFeature::UpdateCharacterSkillIntentResolver(eNsECS::EntityMgr& ecs)
{
	gNsCharacterControlSkill::UpdateCharacterSkillIntentResovlver(ecs);
}