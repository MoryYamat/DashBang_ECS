#include "CharacterSkillFeature.hpp"

#include "Game/Character/Control/Skill/System/CharacterSkillExecutionSystem.hpp"

#include "Game/Character/Control/Skill/System/CharacterSkillIntentResolver.hpp"



// キャラクターのスキル実行を更新する
void Game::Feature::Character::CharacterSkillFeature::UpdateCharacterSkillExecution(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	// Game::Character::Control::Skill::UpdateCharacterSkillExecution(ecs, deltaTime);// 削除予定：FSM導入後廃止
}

// キャラクターのスキル意図を反映可能かどうか判定し，更新する
void Game::Feature::Character::CharacterSkillFeature::UpdateCharacterSkillIntentResolver(Engine::ECS::EntityMgr& ecs)
{
	// Game::Character::Control::Skill::UpdateCharacterSkillIntentResovlver(ecs);// 削除予定：FSM導入後廃止
	Game::Character::Control::Skill::UpdateSkillResolverSystem(ecs);
}