// conditions that are composed of other conditions

#pragma once

#include "Game/Common/Logic/Condition/Condition.hpp"
#include <memory>
#include <vector>

namespace Game::Common::Logic::Condition
{
	// AND条件を評価するクラス
	struct AndCondition : ICondiition
	{
		std::vector<std::unique_ptr<ICondition>> children;

		bool evaluate(const RuntimeContext& ctx) const override
		{
			for (const auto& cond : children)
			{
				if (!cond->evaluate(ctx))
					return false; // いずれかの条件がfalseなら全体もfalse
			}
			return true; // 全ての条件がtrueなら全体もtrue
		}
	};

	// OR条件を評価するクラス
	struct OrCondition : ICondition
	{
		std::vector<std::unique_ptr<ICondition>> children;
		bool evaluate(const RuntimeContext& ctx) const override
		{
			for (const auto& cond : children)
			{
				if (cond->evaluate(ctx))
					return true; // いずれかの条件がtrueなら全体もtrue
			}
			return false; // 全ての条件がfalseなら全体もfalse
		}
	};

	// NOT条件を評価するクラス
	struct NotCondition : ICondition
	{
		std::unique_ptr<ICondition> child;
		bool evaluate(const RuntimeContext& ctx) const override
		{
			return !child->evaluate(ctx); // 子条件の評価結果を反転
		}
	};
}