#pragma once

#include "Game/Combat/Skill/FSM/Definition/SkillFSMContext.hpp"


namespace Game::Combat::Skill::FSM
{

	// スキルの状態遷移条件を定義するインターフェース
	struct ISkillFSMCondition
	{
		virtual ~ISkillFSMCondition() = default;// デフォルト
		virtual bool evaluate(const SkillFSMContext& ctx) const = 0;// 純粋仮想関数
	};

	struct CastTimeElapsed : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext& ctx) const override
		{
			if (!ctx.skillDef) return false;
			return ctx.elapsedTime >= ctx.skillDef->castDuration;
		}
	};

	struct ActiveTimeElapsed : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext& ctx) const override
		{
			if (!ctx.skillDef) return false;
			return ctx.elapsedTime >= ctx.skillDef->activeDuration;
		}
	};

	struct RecoveryTimeElapsed : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext& ctx) const override
		{
			if (!ctx.skillDef) return false;
			return ctx.elapsedTime >= ctx.skillDef->recoveryDuration;
		}
	};

	struct IsInterrupted : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext& ctx) const override
		{
			if (!ctx.skillDef) return false;
			return ctx.isInterrupted;
		}
	};

	// 
	struct Always : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext&) const override { return true; }
	};
}