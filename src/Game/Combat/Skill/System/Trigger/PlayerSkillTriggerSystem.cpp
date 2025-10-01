#include "PlayerSkillTriggerSystem.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Engine/ECS/Component/Tags/PlayerCharacterTag.h"

#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"
#include "Game/Combat/Skill/Intent/Component/SkillIntentComponent.h"

#include "Game/ECS/Tags/CharacterAttribTags.h"

#include "Game/Combat/Skill/Component/SkillSlotAssignmentComponent.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <iostream>

// 削除予定：未使用
// Intentを評価してSkillIntstanceを生成するように改修
void Game::Combat::Skill::Trigger::PlayerSkillTriggerSystem::TriggerPlayerSkillsFromIntent(eNsECS::EntityMgr& ecs)
{

	namespace Ops = Engine::ECS::Ops;

	for (eNsECS::Entity ePlayer : ecs.view<
		gNsSkillIntent::SkillIntentComponent,
		gNsSkillComp::SkillSlotAssignmentComponent,
		eNsLogic2DComp::Logic2DTransformComponent,
		gNsTags::PlayerCharacterTag>())
	{
		const auto& intent = ecs.get<gNsSkillIntent::SkillIntentComponent>(ePlayer);
		const auto& slotAssign = ecs.get<gNsSkillComp::SkillSlotAssignmentComponent>(ePlayer);
		const auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(ePlayer);

		if (!intent.isActive) continue;

		// 値の参照の正統性を改善する(将来的に)
		// スキルIDはいつも`スキル定義(SkillDefinition)`と等しいほうが正統的だと思われる????
		// 
		for (auto slot : intent.requestedSlots)
		{
			//// contains に変更可能？
			auto it = slotAssign.slotToSkillId.find(slot);
			if (it == slotAssign.slotToSkillId.end()) continue;
			// if (!slotAssign.slotToSkillId.contains(slot)) continue;

			 int skillId = it->second;
			//int skillId = slotAssign.slotToSkillId.at(slot);

			// すでに該当スキルが発動中かどうかチェック
			bool alreadyCasting = false;

			// 効率問題 キャッシュ
			for (eNsECS::Entity eSkill : ecs.view<gNsSkillComp::SkillInstanceComponent>())
			{
				const auto& skillInstance = ecs.get<gNsSkillComp::SkillInstanceComponent>(eSkill);
				if (skillInstance.caster == ePlayer && skillInstance.skillId == skillId)
				{
					alreadyCasting = true;
					break;
				}
			}
			
			if (!alreadyCasting)
			{
				// SkillInstance を生成
				eNsECS::Entity skillEntity = ecs.createEntity();

				gNsSkillComp::SkillInstanceComponent skillInstance;
				skillInstance.caster = ePlayer;
				skillInstance.skillId = skillId;
				skillInstance.timeSinceCast = 0.0f;
				Ops::Add<Game::Combat::Skill::Component::SkillInstanceComponent>(ecs, skillEntity, skillInstance);

				eNsLogic2DComp::Transform2DComponent transform2DComp;
				transform2DComp.positionXZ = logic.positionXZ;
				transform2DComp.rotationY = logic.GetRotationYFromFrontVector();
				transform2DComp.front = logic.front;
				transform2DComp.right = logic.right;
				transform2DComp.scale = 1.0f;
				Ops::Add<Engine::ECS::Component::Logic2D::Transform2DComponent>(ecs, skillEntity, transform2DComp);

				std::cout << "[SkillTrigger] Entity " << ePlayer.id
					<< " triggered skill " << skillId
					<< " via slot " << static_cast<int>(slot)
					<< std::endl;
			}

		}
	}
}

// InputActionをトリガーにして直接スキルを生成している(25/06/04)-> 意図層(SkillIntent)を介してトリガーするように変更する必要がある
// 
//void Game::Combat::Skill::Trigger::PlayerSkillTriggerSystem::TriggerSkillsFromInput(eNsECS::EntityMgr& ecs, SkillInputMap& inputMap)
//{
//	for (eNsECS::Entity ePlayer : ecs.view<eNsTagComp::PlayerControllerComponent, gNsInput::InputActionComponent, gNsSkillComp::SkillSlotAssignmentComponent>())
//	{
//		const auto& input = ecs.get<gNsInput::InputActionComponent>(ePlayer);
//		auto& slotAssign = ecs.get<gNsSkillComp::SkillSlotAssignmentComponent>(ePlayer);
//
//		for (const auto& [action, slot] : inputMap.getAllBindings())
//		{
//			if (!input.isPressed(action))
//				continue;
//
//			// containsで省略可能のはず(C++20)
//			auto it = slotAssign.slotToSkillId.find(slot);
//			if (it == slotAssign.slotToSkillId.end())
//				continue;
//
//			int skillId = it->second;
//
//			bool alreadyCasting = false;
//
//			// SkillInstanceが存在するかチェック
//			for (eNsECS::Entity eSkill : ecs.view<gNsSkillComp::SkillInstanceComponent>())
//			{
//				const auto& skillInstance = ecs.get<gNsSkillComp::SkillInstanceComponent>(eSkill);
//				if (skillInstance.caster == ePlayer && skillInstance.skillId == skillId)
//				{
//					// SkillInstance のCasterとPlayerEntityが整合すれば，alreadyCasting
//					alreadyCasting = true;
//					break;
//				}
//			}
//
//			// まだ，alreadyCasting = falseの場合(まだSkillInstanceが存在しない場合)作成する
//			if (!alreadyCasting)
//			{
//				// Transformも生成
//				eNsECS::Entity skillEntity = ecs.createEntity();
//				gNsSkillComp::SkillInstanceComponent skillInstance;
//				skillInstance.caster = ePlayer;
//				skillInstance.skillId = skillId;
//				skillInstance.timeSinceCast = 0.0f;
//				ecs.addComponent(skillEntity, skillInstance);
//
//				std::cout << "[SkillTrigger] Player " << ePlayer.id
//					<< " triggered skill " << skillId
//					<< " via slot " << static_cast<int>(slot)
//					<< std::endl;
//
//				const auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(ePlayer);
//				eNsLogic2DComp::Transform2DComponent transform2DComp;
//				transform2DComp.positionXZ = logic.positionXZ;
//				transform2DComp.rotationY = logic.GetRotationYFromFrontVector();// radians
//				transform2DComp.scale = 1.0f;
//				ecs.addComponent(skillEntity, transform2DComp);
//
//				std::cout << "[PlayerSkillTriggerSystem.cpp(rotation)] rotation Y " << transform2DComp.rotationY << std::endl;
//			}
//		}	
//	}
//}

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