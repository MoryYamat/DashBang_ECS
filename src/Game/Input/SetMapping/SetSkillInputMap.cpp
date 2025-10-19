#include "SetSkillInputMap.h"



#include "Game/Combat/Skill/System/Trigger/SkillInputMap.h"

#include "Game/Character/Control/Skill/MapData/SkillInputMap.hpp"

void Game::Input::Setting::InitSkillInputMap(Engine::ECS::EntityMgr& ecs)
{
	auto& skillMap = ecs.createResource<Game::Character::Control::Skill::SkillInputMap>();

	skillMap.bind(Game::Input::InputAction::CastSkill1, Game::Combat::Skill::Data::SkillSlot::Primary);// スキルID 1
	skillMap.bind(Game::Input::InputAction::CastSkill2, Game::Combat::Skill::Data::SkillSlot::Secondary);// スキルID 2
	skillMap.bind(Game::Input::InputAction::CastSkill3, Game::Combat::Skill::Data::SkillSlot::Utility1);// スキルID 3
	skillMap.bind(Game::Input::InputAction::CastSkill4, Game::Combat::Skill::Data::SkillSlot::Utility2);// スキルID 3
}