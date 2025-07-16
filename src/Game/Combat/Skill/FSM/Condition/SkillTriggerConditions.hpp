#pragma once

#include "Game/Combat/Skill/FSM/Definition/SkillStateComponent.hpp"
#include "Game/Combat/Skill/MasterData/SkillEntry.hpp"
#include "ISkillTriggerCondition.hpp"

#include <optional>
#include <memory>
#include <cstdint>
#include <typeindex>

namespace Game::Combat::Skill::FSM::Condition
{
	using namespace Game::Combat::Skill::FSM;
	using namespace Game::Combat::Skill::Data;

	// フェーズを指定してトリガー条件を設定
	struct SkillTriggerCondition_PhaseEquals : public ISkillTriggerCondition
	{
		std::type_index requiredPhase;

		explicit SkillTriggerCondition_PhaseEquals(std::type_index phase)
			: requiredPhase(phase) { }

		bool evaluate(const SkillStateComponent& state, const SkillDef&) const override
		{
			return state.current == requiredPhase;
		}
	};


	//// フェーズ経過時間の条件
	//struct SkillTriggerCondition_PhaseElapsedTime : public ISkillTriggerCondition
	//{
	//	float minSeconds = 0.0f;
	//	float maxSeconds = std::numeric_limits<float>::max();
	//	std::optional<std::type_index> requiredPhase;

	//	explicit SkillTriggerCondition_PhaseElapsedTime(float minSec, float maxSec, std::optional<std::type_index> phase = std::nullopt)
	//		: minSeconds(minSec), maxSeconds(maxSec), requiredPhase(phase) { }

	//	bool evaluate(const SkillStateComponent& state, const SkillFSMContext& ctx, const SkillDef&) const override
	//	{
	//		if (requiredPhase && state.current != *requiredPhase) return false;
	//		return ctx.phaseElapsedTime >= minSeconds && ctx.phaseElapsedTime <= maxSeconds;
	//	}
	//};

	// 常に真の条件
	struct SkillTriggerCondition_AlwaysTrue : public ISkillTriggerCondition
	{
		bool evaluate(const SkillStateComponent& , const SkillDef&) const override
		{
			return true;
		}
	};

	//// 任意のラムダや関数を渡す
	//struct SkillTriggerCondition_CustomFunction : public ISkillTriggerCondition
	//{
	//	std::function<bool(const SkillStateComponent&, const SkillFSMContext&, const SkillDef&)> func;

	//	explicit SkillTriggerCondition_CustomFunction(decltype(func) f) : func(std::move(f)) {}

	//	bool evaluate(const SkillStateComponent& state, const SkillFSMContext& ctx, const SkillDef& def) const override
	//	{
	//		return func(state, ctx, def);
	//	}

	//};
}