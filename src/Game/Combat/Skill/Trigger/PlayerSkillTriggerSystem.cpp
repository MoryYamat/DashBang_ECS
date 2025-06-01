#include "PlayerSkillTriggerSystem.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"

#include "Game/Combat/Skill/Component/SkillSlotAssignmentComponent.h"

#include <iostream>


void Game::Combat::Skill::Trigger::PlayerSkillTriggerSystem::TriggerSkillsFromInput(eNsECS::EntityMgr& ecs, SkillInputMap& inputMap)
{
	for (eNsECS::Entity ePlayer : ecs.view<eNsTagComp::PlayerControllerComponent, gNsInput::InputActionComponent, gNsSkillComp::SkillSlotAssignmentComponent>())
	{
		const auto& input = ecs.get<gNsInput::InputActionComponent>(ePlayer);
		auto& slotAssign = ecs.get<gNsSkillComp::SkillSlotAssignmentComponent>(ePlayer);

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
			for (eNsECS::Entity eSkill : ecs.view<gNsSkillComp::SkillInstanceComponent>())
			{
				const auto& skillInstance = ecs.get<gNsSkillComp::SkillInstanceComponent>(eSkill);
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
				// Transformも生成
				eNsECS::Entity skillEntity = ecs.createEntity();
				gNsSkillComp::SkillInstanceComponent skillInstance;
				skillInstance.caster = ePlayer;
				skillInstance.skillId = skillId;
				skillInstance.timeSinceCast = 0.0f;
				ecs.addComponent(skillEntity, skillInstance);

				std::cout << "[SkillTrigger] Player " << ePlayer.id
					<< " triggered skill " << skillId
					<< " via slot " << static_cast<int>(slot)
					<< std::endl;

				const auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(ePlayer);
				eNsLogic2DComp::Transform2DComponent transform2DComp;
				transform2DComp.positionXZ = logic.positionXZ;
				transform2DComp.rotationY = logic.GetRotationYFromFrontVector();// radians
				transform2DComp.scale = 1.0f;
				ecs.addComponent(skillEntity, transform2DComp);

				std::cout << "[PlayerSkillTriggerSystem.cpp(rotation)] rotation Y " << transform2DComp.rotationY << std::endl;
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