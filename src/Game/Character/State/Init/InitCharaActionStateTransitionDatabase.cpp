#include "InitCharaActionStateTransitionDatabase.hpp"


#include "Game/Character/State/Rule/Action/ActionStateTransitionDatabase.hpp"


#include "Common/GameNamespaceDecl.h"

void Game::Character::State::Action::InitCharaActionStateTransitionDatabase(eNsECS::EntityMgr& ecs)
{
	auto& db = ecs.createResource<gNsCharaActionState::StateTransitionDatabase>();

	using State = gNsCharaActionState::ActionState;
	using ConditionType = gNsCharaActionState::TransitionConditionType;

	db.rules =
	{
		// None → SkillCasting
		{
			.from = State::None,
			.to = State::SkillCasting,
			.conditions = {
				{ ConditionType::SkillTriggered }
			}
		},
		// SkillCasting → None（時間経過）
		{
			.from = State::SkillCasting,
			.to = State::None,
			.conditions = {
				{ ConditionType::ActionDurationElapsed, 1.0f } // 仮：スキル発動1秒後終了
			}
		},

		//// None → Rolling
		//{
		//	.from = State::None,
		//	.to = State::Rolling,
		//	.conditions = {
		//		{ ConditionType::RequestRoll }
		//	}
		//},
		//// Rolling → None（回避終了）
		//{
		//	.from = State::Rolling,
		//	.to = State::None,
		//	.conditions = {
		//		{ ConditionType::IsTimeElapsed, 0.5f } // 仮：回避時間
		//	}
		//},

		//// None → Guarding
		//{
		//	.from = State::None,
		//	.to = State::Guarding,
		//	.conditions = {
		//		{ ConditionType::RequestGuard }
		//	}
		//},


		//// Guarding → None（ガード解除）
		//{
		//	.from = State::Guarding,
		//	.to = State::None,
		//	.conditions = {
		//		{ ConditionType::IsTimeElapsed, 1.0f } // 仮：ガード継続時間
		//	}
		//},

		//// None → UsingItem
		//{
		//	.from = State::None,
		//	.to = State::UsingItem,
		//	.conditions = {
		//		{ ConditionType::RequestGuard } // 仮：アイテム使用もガードと同じ条件で
		//	}
		//},
		//// UsingItem → None（アイテム使用終了）
		//{
		//	.from = State::UsingItem,
		//	.to = State::None,
		//	.conditions = {
		//		{ ConditionType::ActionDurationElapsed, 1.0f } // 仮：アイテム使用時間
		//	}
		//}
	};

	std::cout << "[InitCharaActionStateTransitionDatabase.cpp]: Action state transition condition database initialization completed successfully.\n";

}