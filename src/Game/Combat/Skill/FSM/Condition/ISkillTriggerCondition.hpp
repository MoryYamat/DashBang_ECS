#pragma once

#include "Game/Combat/Skill/FSM/Definition/SkillStateComponent.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"
#include "Game/Combat/Skill/FSM/Definition/SkillFSMContext.hpp"

namespace Game::Combat::Skill::FSM::Condition
{
	using namespace Game::Combat::Skill::FSM;
	using namespace Game::Combat::Skill::Def;

	// スキルトリガー可否判定は，現在のSkillState,実行中のSkillDef,
	// トリガーしようと(意図)しようとしている，SkillDef,によって判定されるべき．
	// => SkillFSMは内部挙動の制御ロジックであって，外部FSM定義自体に左右されるべきではない

	// スキルトリガーの判定条件インターフェース
	struct ISkillTriggerCondition
	{
		virtual ~ISkillTriggerCondition() = default;

		// 
		virtual bool evaluate(const SkillStateComponent& state, const SkillDef& def) const = 0;
	};
}