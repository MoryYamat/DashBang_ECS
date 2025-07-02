//

#pragma once

#include <functional>

namespace Game::Common::Logic::Condition
{
	enum class CompareOperator
	{
		Equal,		// 等しい
		NotEqual, // 等しくない
		Less, 		// より小さい
		LessEqual, // 以下
		Greater, 	// より大きい
		GreaterEqual // 以上
	};

	// lhs [] rhs の比較を行う条件クラス
	template<typename T>
	struct CompareCondition: ICondition
	{ 
		std::function<T(const RuntimeContext&)> lhsGetter;// 左辺の値を取得する関数`[](const RuntimeContext& ctx){return ctx.value}`
		CompareOperator op; // 比較演算子
		std::function<T(const RuntimeContext&)> rhsGetter;// 右辺の値を取得する関数(定数の場合`[](){return 1.0f etc.}`)


		bool evaluate(const RuntimeContext& ctx) const override
		{
			T lhs = lhsGetter(ctx);
			T rhs = rhsGetter(ctx);
			switch (op)
			{
			case CompareOperator::Equal: return lhs == rhs;
			case CompareOperator::NotEqual: return lhs != rhs;
			case CompareOperator::Less: return lhs < rhs;
			case CompareOperator::LessEqual: return lhs <= rhs;
			case CompareOperator::Greater: return lhs > rhs;
			case CompareOperator::GreaterEqual: return lhs >= rhs;
			}
			return false; // デフォルトはfalse
		}

	};
}