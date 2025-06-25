#include "CharacterSkillExecutionSystem.hpp"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Engine/ECS/Component/Tags/PlayerCharacterTag.h"

#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"

#include "Game/Character/Control/Skill/Intent/SkillIntentComponent.hpp"

#include "Game/ECS/Tags/CharacterAttribTags.h"

#include "Game/Combat/Skill/Component/SkillSlotAssignmentComponent.h"

#include <iostream>

#include "Common/GameNamespaceDecl.h"

// FIXME:Intentに基づいてキャラクターのスキルを実行する．※本来はこのExecutionの前にResolverによる実行可否判定が必要
// Intent -> Resolver -> Logic : IntentとStateに基づいてスキルの実行可否を判定する
void Game::Character::Control::Skill::UpdateCharacterSkillExecution(eNsECS::EntityMgr& ecs, float deltaTime)
{
	for (eNsECS::Entity ePlayer : ecs.view<
		gNsCharacterControlSkill::SkillIntentComponent,
		gNsSkillComp::SkillSlotAssignmentComponent,
		eNsLogic2DComp::Logic2DTransformComponent,
		gNsTags::PlayerCharacterTag>())
	{
		const auto& intent = ecs.get<gNsCharacterControlSkill::SkillIntentComponent>(ePlayer);
		const auto& slotAssign = ecs.get<gNsSkillComp::SkillSlotAssignmentComponent>(ePlayer);
		const auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(ePlayer);

		// ボタンが押されていない場合はスルー
		if (!intent.isActive) continue;

		// 値の参照の正統性を改善する(将来的に)
		// スキルIDはいつも`スキル定義(SkillDefinition)`と等しいほうが正統的だと思われる????
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
			// Fixme: 同じスキルを短時間で複数回発動(連射みたいな感じ)できるようにする場合は、ここを変更する必要がある
			for (eNsECS::Entity eSkill : ecs.view<gNsSkillComp::SkillInstanceComponent>())
			{
				const auto& skillInstance = ecs.get<gNsSkillComp::SkillInstanceComponent>(eSkill);
				if (skillInstance.caster == ePlayer && skillInstance.skillId == skillId)
				{
					// すでにスキルが発動中
					alreadyCasting = true;
					break;
				}
			}

			// Fixme: このフラグでは同じスキルを重複発動できないので，別の方法を検討する必要がある
			if (!alreadyCasting)
			{
				// SkillInstance を生成
				eNsECS::Entity skillEntity = ecs.createEntity();

				gNsSkillComp::SkillInstanceComponent skillInstance;
				skillInstance.caster = ePlayer;
				skillInstance.skillId = skillId;
				skillInstance.timeSinceCast = 0.0f;
				ecs.addComponent(skillEntity, skillInstance);

				eNsLogic2DComp::Transform2DComponent transform2DComp;
				transform2DComp.positionXZ = logic.positionXZ;
				transform2DComp.rotationY = logic.GetRotationYFromFrontVector();
				transform2DComp.front = logic.front;
				transform2DComp.right = logic.right;
				transform2DComp.scale = 1.0f;
				ecs.addComponent(skillEntity, transform2DComp);

				std::cout << "[SkillTrigger] Entity " << ePlayer.id
					<< " triggered skill " << skillId
					<< " via slot " << static_cast<int>(slot)
					<< std::endl;
			}

		}
	}
}