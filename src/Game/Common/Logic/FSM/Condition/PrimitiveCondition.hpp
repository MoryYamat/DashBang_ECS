//

#pragma once

#include "RuntimeContext.hpp"

#include <variant>
#include <functional>

namespace Game::Common::Logic::Condition
{
	// プリミティブな比較演算子を定義する列挙型
	enum class CompareOperator
	{
		Equal,		// 等しい
		NotEqual, // 等しくない
		Less, 		// より小さい
		LessEqual, // 以下
		Greater, 	// より大きい
		GreaterEqual // 以上
	};

	// 
	//struct CompareCondition : ICondition
	//{
	//	RuntimeKey key;
	//	CompareOperator op; // 比較演算子
	//	RuntimeValue rhs;

	//	bool evaluate(const RuntimeContext& ctx) const override
	//	{
	//		auto it = ctx.values.find(key);
	//		if (it == ctx.values.end()) return false; // キーが存在しない場合はfalse

	//		return std::visit([&](auto&& lhs) -> bool
	//			{
	//				using T = std::decay_t<decltype(lhs)>;// 左辺の型を取得
	//				if (auto* r = std::get_if<T>(&rhs))// 右辺の型が左辺と一致する場合
	//				{
	//					switch (op)
	//					{
	//					case CompareOperator::Equal: return lhs == *r;
	//					case CompareOperator::NotEqual: return lhs != *r;
	//					case CompareOperator::Less: return lhs < *r;
	//					case CompareOperator::LessEqual: return lhs <= *r;
	//					case CompareOperator::Greater: return lhs > *r;
	//					case CompareOperator::GreaterEqual: return lhs >= *r;
	//					}
	//				}
	//				return false; // rhsの型が一致しない場合はfalse
	//			}, it->second);
	//	}
	//}


	// std::unique_ptr<ICondition> makeHPConditionGE(int threshold) {
	// 	return std::make_unique<CompareCondition>(CompareCondition{
	// 		.key = RuntimeKey::CurrentHP,
	// 		.op = CompareOperator::GreaterEqual,
	// 		.rhs = threshold
	// 		});
	// }

	//// lhs [] rhs の比較を行う条件クラス
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

	// 他の比較条件を定義する場合，IConditionを継承して式を定義することができる．

	// ランダムな確率で条件を満たすかどうかを評価するクラス
	struct RandomChanceCondition : ICondition {
		float chance = 0.0f; // 0.0〜1.0
		bool evaluate(const RuntimeContext& ctx) const override {
			return static_cast<float>(rand()) / RAND_MAX < chance;
		}
	};

}