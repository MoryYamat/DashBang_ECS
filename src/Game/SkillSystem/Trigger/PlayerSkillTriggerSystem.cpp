#include "PlayerSkillTriggerSystem.h"

#include "Core/ECS/Component/Tags/PlayerControllerComponent.h"

#include "Game/SkillSystem/Component/SkillInstanceComponent.h"

#include "Game/SkillSystem/Component/SkillSlotAssignmentComponent.h"

#include <iostream>


void SkillSystem::Trigger::PlayerSkillTriggerSystem::TriggerSkillsFromInput(ECS& ecs, SkillInputMap& inputMap)
{
	for (Entity ePlayer : ecs.view<PlayerControllerComponent, InputActionComponent, SkillSlotAssignmentComponent>())
	{
		const auto& input = ecs.get<InputActionComponent>(ePlayer);
		auto& slotAssign = ecs.get<SkillSlotAssignmentComponent>(ePlayer);

		for (const auto& [action, slot] : inputMap.getAllBindings())
		{
			if (!input.isPressed(action))
				continue;

			auto it = slotAssign.slotToSkillId.find(slot);
			if (it == slotAssign.slotToSkillId.end())
				continue;

			int skillId = it->second;

			bool alreadyCasting = false;

			// SkillInstanceが存在するかチェック
			for (Entity eSkill : ecs.view<SkillInstanceComponent>())
			{
				const auto& skillInstance = ecs.get<SkillInstanceComponent>(eSkill);
				if (skillInstance.caster == ePlayer && skillInstance.skillId == skillId)
				{
					// SkillInstance のCasterとPlayerEntityが整合すれば，alreadyCasting
					alreadyCasting = true;
					break;
				}
			}

			// まだ，alreadyCasting = falseの場合(まだSkillInstanceが存在しない場合)作成する
			if (!alreadyCasting)
			{
				Entity skillEntity = ecs.createEntity();
				SkillInstanceComponent skillInstance;
				skillInstance.caster = ePlayer;
				skillInstance.skillId = skillId;
				skillInstance.timeSinceCast = 0.0f;
				ecs.addComponent(skillEntity, skillInstance);

				std::cout << "[SkillTrigger] Player " << ePlayer.id
					<< " triggered skill " << skillId
					<< " via slot " << static_cast<int>(slot)
					<< std::endl;
			}
		}	
	}
}

//void SkillSystem::Trigger::PlayerSkillTriggerSystem::Update(ECS& ecs, SkillInputMap& SkillInputMap)
//{
//	for (Entity e : ecs.view<PlayerControllerComponent, InputActionComponent>())
//	{
//		const auto& input = ecs.get<InputActionComponent>(e);
//
//		for (const auto& [action, skillID] : SkillInputMap.getAllBindings())
//		{
//			if (input.isPressed(action))
//			{
//				// 重複発動防止
//				bool alreadyCasting = false;
//				for (Entity eSkill : ecs.view<SkillInstanceComponent>())
//				{
//					auto& skill = ecs.get<SkillInstanceComponent>(eSkill);
//					if (skill.caster == e && skill.skillId == skillID)
//					{
//						alreadyCasting = true;
//						break;
//					}
//				}
//
//				if (!alreadyCasting)
//				{
//					Entity skillEntity = ecs.createEntity();
//					SkillInstanceComponent activeSkillComp;
//					activeSkillComp.caster = e;
//					activeSkillComp.skillId = skillID;
//					activeSkillComp.timeSinceCast = 0.0f;
//					ecs.addComponent(skillEntity, activeSkillComp);
//					std::cout << "[SkillTrigger] Player triggered skill " << skillID << " from action " << static_cast<int>(action) << std::endl;
//				}
//			}
//		}
//
//	}
//}