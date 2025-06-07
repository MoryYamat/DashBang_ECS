#include "SetSkillInputMap.h"

#include "Common/GameNamespaceDecl.h"

#include "Game/Combat/Skill/Trigger/SkillInputMap.h"

void Game::Input::Setting::InitSkillInputMap(eNsECS::EntityMgr& ecs)
{
	auto& skillMap = ecs.createResource<gNsSkillTrigger::SkillInputMap>();

	skillMap.bind(gNsInput::InputAction::CastSkill1, gNsSkillData::SkillSlot::Primary);// スキルID 1
	skillMap.bind(gNsInput::InputAction::CastSkill2, gNsSkillData::SkillSlot::Secondary);// スキルID 2
	skillMap.bind(gNsInput::InputAction::CastSkill3, gNsSkillData::SkillSlot::Utility1);// スキルID 3
}