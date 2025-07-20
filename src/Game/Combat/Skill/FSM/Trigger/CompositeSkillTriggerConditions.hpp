#pragma once


#include "Game/Combat/Skill/FSM/Condition/ISkillTriggerCondition.hpp"

#include <memory>

namespace Game::Combat::Skill::FSM::Condition
{
	using namespace Game::Combat::Skill::FSM::Condition;

	// And condition
	struct SkillTriggerCondition_And : public ISkillTriggerCondition
	{
		std::vector<std::shared_ptr<ISkillTriggerCondition>> conditions;

		explicit SkillTriggerCondition_And(std::vector<std::shared_ptr<ISkillTriggerCondition>> conds)
			: conditions(std::move(conds)) { }

		bool evaluate(const SkillStateComponent& state, const SkillDef& def) const override
		{
			for (const auto& cond : conditions)
			{
				if (!cond->evaluate(state, def)) return false;
			}
			return true;
		}
	};

	// OR condition
	struct SkillTriggerCondition_OR : public ISkillTriggerCondition
	{
		std::vector<std::shared_ptr<ISkillTriggerCondition>> conditions;

		explicit SkillTriggerCondition_OR(std::vector<std::shared_ptr<ISkillTriggerCondition>> conds)
			: conditions(std::move(conds)) { }

		bool evaluate(const SkillStateComponent& state, const SkillDef& def)
		{
			for (const auto& cond : conditions)
				if (cond->evaluate(state, def)) return true;

			return false;
		}
	};

	// NOT condition
	struct SkillTriggerCondition_Not : public ISkillTriggerCondition
	{
		std::shared_ptr<ISkillTriggerCondition> condition;

		explicit SkillTriggerCondition_Not(std::shared_ptr<ISkillTriggerCondition> cond)
			: condition(std::move(cond)) { }

		bool evaluate(const SkillStateComponent& state, const SkillDef& def)
		{
			return !condition->evaluate(state, def);
		}
	};
}