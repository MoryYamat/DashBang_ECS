#include "CharacterSkillExecutionSystem.hpp"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"


// 廃止予定：SkillExecutionComponentに移行
#include "Game/Combat/Private/Skill/Component/SkillInstanceComponent.h"

#include "Game/Combat/Private/Skill/Component/SkillExecutionContextComponent.hpp"

#include "Game/Character/Private/Control/Skill/Intent/SkillIntentComponent.hpp"

#include "Game/ECS/Private/Tags/CharacterAttribTags.h"

#include "Game/Combat/Private/Skill/Component/SkillSlotAssignmentComponent.h"

#include <iostream>



// 削除予定：FSM実装後廃止予定
// FIXME:Intentに基づいてキャラクターのスキルを実行する．※本来はこのExecutionの前にResolverによる実行可否判定が必要
// Intent -> Resolver -> Logic : IntentとStateに基づいてスキルの実行可否を判定する
void Game::Character::Control::Skill::UpdateCharacterSkillExecution(Engine::ECS::EntityMgr& ecs, float deltaTime)
{

	//for (Engine::ECS::Entity ePlayer : ecs.view<
	//	Game::Character::Control::Skill::SkillIntentComponent,
	//	Game::Combat::Skill::Component::SkillSlotAssignmentComponent,
	//	Engine::ECS::Component::Logic2D::Logic2DTransformComponent,
	//	Game::ECS::Tags::PlayerCharacterTag>())
	//{
	//	const auto& intent = ecs.get<Game::Character::Control::Skill::SkillIntentComponent>(ePlayer);
	//	const auto& slotAssign = ecs.get<Game::Combat::Skill::Component::SkillSlotAssignmentComponent>(ePlayer);
	//	const auto& logic = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(ePlayer);

	//	// ボタンが押されていない場合はスルー
	//	if (!intent.isActive) continue;

	//	// 値の参照の正統性を改善する(将来的に)
	//	// スキルIDはいつも`スキル定義(SkillDefinition)`と等しいほうが正統的だと思われる????
	//	for (auto slot : intent.requestedSlots)
	//	{
	//		//// contains に変更可能？
	//		auto it = slotAssign.slotToSkillId.find(slot);
	//		if (it == slotAssign.slotToSkillId.end()) continue;
	//		// if (!slotAssign.slotToSkillId.contains(slot)) continue;

	//		int skillId = it->second;
	//		//int skillId = slotAssign.slotToSkillId.at(slot);

	//		// すでに該当スキルが発動中かどうかチェック
	//		bool alreadyCasting = false;

	//		// 効率問題 キャッシュ
	//		// Fixme: 同じスキルを短時間で複数回発動(連射みたいな感じ)できるようにする場合は、ここを変更する必要がある
	//		for (Engine::ECS::Entity eSkill : ecs.view<Game::Combat::Skill::Component::SkillInstanceComponent>())
	//		{
	//			const auto& skillInstance = ecs.get<Game::Combat::Skill::Component::SkillInstanceComponent>(eSkill);
	//			if (skillInstance.caster == ePlayer && skillInstance.skillId == skillId)
	//			{
	//				// すでにスキルが発動中
	//				alreadyCasting = true;
	//				break;
	//			}
	//		}

	//		// Fixme: このフラグでは同じスキルを重複発動できないので，別の方法を検討する必要がある
	//		if (!alreadyCasting)
	//		{
	//			// SkillInstance を生成
	//			Engine::ECS::Entity skillEntity = ecs.createEntity();

	//			Game::Combat::Skill::Component::SkillExecutionComponent execution;
	//			execution.caster = ePlayer;
	//			execution.skillId = skillId;
	//			execution.currentPhase = Game::Combat::Skill::Component::SkillExecutionPhase::Casting;
	//			execution.timeSinceCast = 0.0f;
	//			execution.phaseElapsedTime = 0.0f;
	//			execution.isInterrupted = false;
	//			ecs.addComponent(skillEntity, execution);

	//			Engine::ECS::Component::Logic2D::Transform2DComponent transform2DComp;
	//			transform2DComp.positionXZ = logic.positionXZ;
	//			transform2DComp.rotationY = logic.GetRotationYFromFrontVector();
	//			transform2DComp.front = logic.front;
	//			transform2DComp.right = logic.right;
	//			transform2DComp.scale = 1.0f;
	//			ecs.addComponent(skillEntity, transform2DComp);

	//			std::cout << "[SkillTrigger] Entity " << ePlayer.id
	//				<< " triggered skill " << skillId
	//				<< " via slot " << static_cast<int>(slot)
	//				<< std::endl;
	//		}

	//	}
	//}
}


// FIXME:Intentに基づいてキャラクターのスキルを実行する．※本来はこのExecutionの前にResolverによる実行可否判定が必要
// Intent -> Resolver -> Logic : IntentとStateに基づいてスキルの実行可否を判定する
//void Game::Character::Control::Skill::UpdateCharacterSkillExecution(Engine::ECS::EntityMgr& ecs, float deltaTime)
//{
//
//	for (Engine::ECS::Entity ePlayer : ecs.view<
//		Game::Character::Control::Skill::SkillIntentComponent,
//		Game::Combat::Skill::Component::SkillSlotAssignmentComponent,
//		Engine::ECS::Component::Logic2D::Logic2DTransformComponent,
//		Game::ECS::Tags::PlayerCharacterTag>())
//	{
//		const auto& intent = ecs.get<Game::Character::Control::Skill::SkillIntentComponent>(ePlayer);
//		const auto& slotAssign = ecs.get<Game::Combat::Skill::Component::SkillSlotAssignmentComponent>(ePlayer);
//		const auto& logic = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(ePlayer);
//
//		// ボタンが押されていない場合はスルー
//		if (!intent.isActive) continue;
//
//		// 値の参照の正統性を改善する(将来的に)
//		// スキルIDはいつも`スキル定義(SkillDefinition)`と等しいほうが正統的だと思われる????
//		for (auto slot : intent.requestedSlots)
//		{
//			//// contains に変更可能？
//			auto it = slotAssign.slotToSkillId.find(slot);
//			if (it == slotAssign.slotToSkillId.end()) continue;
//			// if (!slotAssign.slotToSkillId.contains(slot)) continue;
//
//			int skillId = it->second;
//			//int skillId = slotAssign.slotToSkillId.at(slot);
//
//			// すでに該当スキルが発動中かどうかチェック
//			bool alreadyCasting = false;
//
//			// 効率問題 キャッシュ
//			// Fixme: 同じスキルを短時間で複数回発動(連射みたいな感じ)できるようにする場合は、ここを変更する必要がある
//			for (Engine::ECS::Entity eSkill : ecs.view<Game::Combat::Skill::Component::SkillInstanceComponent>())
//			{
//				const auto& skillInstance = ecs.get<Game::Combat::Skill::Component::SkillInstanceComponent>(eSkill);
//				if (skillInstance.caster == ePlayer && skillInstance.skillId == skillId)
//				{
//					// すでにスキルが発動中
//					alreadyCasting = true;
//					break;
//				}
//			}
//
//			// Fixme: このフラグでは同じスキルを重複発動できないので，別の方法を検討する必要がある
//			if (!alreadyCasting)
//			{
//				// SkillInstance を生成
//				Engine::ECS::Entity skillEntity = ecs.createEntity();
//
//				Game::Combat::Skill::Component::SkillInstanceComponent skillInstance;
//				skillInstance.caster = ePlayer;
//				skillInstance.skillId = skillId;
//				skillInstance.timeSinceCast = 0.0f;
//				ecs.addComponent(skillEntity, skillInstance);
//
//				Engine::ECS::Component::Logic2D::Transform2DComponent transform2DComp;
//				transform2DComp.positionXZ = logic.positionXZ;
//				transform2DComp.rotationY = logic.GetRotationYFromFrontVector();
//				transform2DComp.front = logic.front;
//				transform2DComp.right = logic.right;
//				transform2DComp.scale = 1.0f;
//				ecs.addComponent(skillEntity, transform2DComp);
//
//				std::cout << "[SkillTrigger] Entity " << ePlayer.id
//					<< " triggered skill " << skillId
//					<< " via slot " << static_cast<int>(slot)
//					<< std::endl;
//			}
//
//		}
//	}
//}

//Engine::ECS::Entity Game::Character::Control::Skill::CreateSkillInstanceWithContext
//(
//	Engine::ECS::EntityMgr& ecs,
//	Engine::ECS::Entity caster,
//	int skillId,
//	const Engine::ECS::Component::Logic2D::Logic2DTransformComponent& casterTransform
//)
//{
//	// skillInstanceを生成
//	Engine::ECS::Entity skillEntity = ecs.createEntity();
//
//	Game::Combat::Skill::Component::SkillInstanceComponent skillInstance;
//	skillInstance.caster = caster;
//	skillInstance.skillId = skillId;
//	skillInstance.timeSinceCast = 0.0f;
//	ecs.addComponent(skillEntity, skillInstance);
//
//	// Transform2DComponentを設定
//	Engine::ECS::Component::Logic2D::Transform2DComponent transform2DComp;
//	transform2DComp.positionXZ = casterTransform.positionXZ;
//	transform2DComp.rotationY = casterTransform.GetRotationYFromFrontVector();
//	transform2DComp.front = casterTransform.front;
//	transform2DComp.right = casterTransform.right;
//	transform2DComp.scale = 1.0f; // スケールは1.0fで初期化
//	ecs.addComponent(skillEntity, transform2DComp);
//
//	// コンテキストへの書き込み
//
//}