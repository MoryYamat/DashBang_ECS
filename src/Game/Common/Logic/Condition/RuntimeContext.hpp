// 条件を評価する際に参照される実行中の情報を保持するための構造体

#pragma once

#include <variant>
#include <unordered_map>

namespace Game::Common::Logic::Condition
{
	// 実行時の値を保持するための型
	using RuntimeValue = std::variant<
		float,
		int,
		bool,
	>;


	// TKeyをキーとして、実行時の値を保持するコンテキスト
	template<typename TKey>
	struct RuntimeContext
	{
		std::unordered_map<TKey, RuntimeValue> values;

		// コンテキストから値を取得するためのテンプレート関数
		template<typename T>
		T get(TKey key) const
		{
			return std::get<T>(values.at(key));
		}

		// コンテキストに値を設定するためのテンプレート関数
		template<typename T>
		void set(TKey key, T value)
		{
			values[key] = value;
		}
	};
}

// enum class SkillRuntimeKey
// {
// 	ElapsedTime,
// 	HitCount
// };
// 
// using SkillRuntimeContext = RuntimeContext<SkillRuntimeKey>;
// 
// SkillRuntimeContext ctx;
// ctx.set<float>(SkillRuntimeKey::ElapsedTime, 1.2f);
// ctx.set<int>(SkillRuntimeKey::HitCount, 3);
// 
// float elapsed = ctx.get<float>(SkillRuntimeKey::ElapsedTime);