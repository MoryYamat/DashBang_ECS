// 条件を評価する際に参照される実行中の情報を保持するための構造体
// Contextデータ：毎フレーム更新されるデータを保持するための構造体


#pragma once

#include <variant>
#include <unordered_map>
#include <optional>

namespace Game::Common::Logic::Codition
{
	// 実行時の値を保持するための型
	// もう少し抽象度が必要ではないか？つまり使用側でこれらを定義するようにすべきではないか？
	//using RuntimeValue = std::variant<
	//	float,
	//	int,
	//	bool,
	//>;

	// キーに対応する型を定義するためのテンプレート構造体
	template<typename... ValueTypes>
	struct RuntimeValueWrapper
	{
		using Value = std::variant<ValueTypes...>;
	};

	// TKeyをキーとして、実行時の値を保持するコンテキスト
	template<typename RuntimeKey, typename ValueWrapper>
	struct RuntimeContext
	{
		using RuntimeValue = typename ValueWrapper::Value;
		std::unordered_map<RuntimeKey, RuntimeValue> values;


		// コンテキストに値を設定するためのテンプレート関数
		template<RuntimeKey key>
		void set(typename KeyTraits<key>::Type value)
		{
			values[key] = value;
		}

		// 固定キーで値を取得するためのテンプレート関数
		template<RuntimeKey key>
		typename KeyTraits<key>::Type get() const
		{
			return std::get<typename KeyTraits<key>::Type>(values.at(key));
		}


		// 動的キーで値を取得するための汎用関数
		template<typename T>
		std::optional<T> try_get(RuntimeKey key) const
		{
			auto it = values.find(key);
			if (it != values.end())// キーが存在する場合
			{
				if (auto* val = std::get_if<T>(&it->second))
					return *val;
			}
			return std::nullopt;
		}
	};


}

// 使用例
// RuntimeKeyを定義
// enum class SkillRuntimeKey {
// 	Cooldown,
// 	CurrentHP,
// 	ElapsedTime,
// };
// 
// RuntimeKeyをキーとして、値の型を定義するためのTraits
// template<SkillRuntimeKey key>
// struct KeyTraits;
// 
// // 特化化を行うことで、各キーに対応する型を定義
// template<> struct KeyTraits<SkillRuntimeKey::Cooldown> {
// 	using Type = float;
// };
// template<> struct KeyTraits<SkillRuntimeKey::CurrentHP> {
// 	using Type = int;
// };
// 
// // 型エイリアスによって，RuntimeValueWrapperを定義
// using SkillValueWrapper = RuntimeValueWrapper<float, int, bool>;
//
// // RuntimeContextを定義
// using SkillRuntimeContext = RuntimeContext<SkillRuntimeKey, SkillValueWrapper>;