// Skill開始(None->Casting)の起動条件

#pragma once

#include "Game/Combat/Skill/FSM/StateModel/SkillStateComponent.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"

namespace Game::Combat::Skill::FSM::Condition
{

	// スキルトリガー可否判定は，現在のSkillState,実行中のSkillDef,
	// トリガーしようと(意図)しようとしている，SkillDef,によって判定されるべき．
	// => SkillFSMは内部挙動の制御ロジックであって，外部FSM定義自体に左右されるべきではない

	// スキルトリガーの判定条件インターフェース
	struct ISkillTriggerCondition
	{
		virtual ~ISkillTriggerCondition() = default;

		// 
		virtual bool evaluate(
			const Game::Combat::Skill::FSM::StateModel::SkillStateComponent& state, 
			const Game::Combat::Skill::Def::SkillDef& def) const = 0;
	};
}