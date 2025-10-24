#pragma once

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Private/Skill/Def/SkillDef.hpp"

namespace Game::Combat::Skill::FSM
{

	// スキルの状態遷移条件を定義するインターフェース
	struct ISkillFSMCondition
	{
		virtual ~ISkillFSMCondition() = default;// デフォルト
		virtual bool evaluate(
			const SkillFSMContext& ctx, 
			const Game::Combat::Skill::Def::SkillDef& def) const = 0;// 純粋仮想関数
	};

	struct CastTimeElapsed : ISkillFSMCondition
	{
		bool evaluate(
			const SkillFSMContext& ctx, const Game::Combat::Skill::Def::SkillDef& def) const override
		{
			return ctx.phaseElapsedTime >= def.castDuration;
		}
	};

	struct ActiveTimeElapsed : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext& ctx, const Game::Combat::Skill::Def::SkillDef& def) const override
		{
			return ctx.phaseElapsedTime >= def.activeDuration;
		}
	};

	struct RecoveryTimeElapsed : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext& ctx, const Game::Combat::Skill::Def::SkillDef& def) const override
		{
			return ctx.phaseElapsedTime >= def.recoveryDuration;
		}
	};

	struct IsInterrupted : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext& ctx, const Game::Combat::Skill::Def::SkillDef& def) const override
		{
			return ctx.isInterrupted;
		}
	};

	// 
	struct AlwaysTrue : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext&, const Game::Combat::Skill::Def::SkillDef&) const override { return true; }
	};

	// Noneにリセットする
	struct ResetToNoneCondition : ISkillFSMCondition
	{
		bool evaluate(const SkillFSMContext&, const Game::Combat::Skill::Def::SkillDef&) const override
		{
			return true;// "to = None"に限定されて使われる前提
		}
	};
}