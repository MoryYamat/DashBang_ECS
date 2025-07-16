#pragma once

#include "Game/Combat/Skill/FSM/Definition/SkillFSMContext.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"

namespace Game::Combat::Skill::FSM
{
	using namespace Game::Combat::Skill::Def;

	// スキルの状態遷移条件を定義するインターフェース
	struct ISkillFSMCondition
	{
		virtual ~ISkillFSMCondition() = default;// デフォルト
		virtual bool evaluate(const SkillFSMContext& ctx, const SkillDef& def) const = 0;// 純粋仮想関数
	};

	struct CastTimeElapsed : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext& ctx, const SkillDef& def) const override
		{
			return ctx.phaseElapsedTime >= def.castDuration;
		}
	};

	struct ActiveTimeElapsed : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext& ctx, const SkillDef& def) const override
		{
			return ctx.phaseElapsedTime >= def.activeDuration;
		}
	};

	struct RecoveryTimeElapsed : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext& ctx, const SkillDef& def) const override
		{
			return ctx.phaseElapsedTime >= def.recoveryDuration;
		}
	};

	struct IsInterrupted : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext& ctx, const SkillDef& def) const override
		{
			return ctx.isInterrupted;
		}
	};

	// 
	struct Always : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext&, const SkillDef&) const override { return true; }
	};
}