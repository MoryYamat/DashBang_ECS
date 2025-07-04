// 

// やること
// ~~IConditionの再検討：拡張性・型安全性・柔軟性を再整理~~
// FSMInstanceの設計: 状態遷移の実行を管理するインスタンス
// FSMExecutorの設計: 実行機構．定義に基づいて実際に状態遷移を行う
// IConditionFactoryの設計: データ定義-> 条件インスタンス変換のためのファクトリ
// 並列FSMをどうまとめて制御するか: 
// 

#pragma once

#include "Game/Common/Logic/FSM/Condition/IConditrion.hpp"
#include <vector>
#include <optional>
#include <memory>
#include <variant>
#include <string>

namespace Game::Common::Logic::FSM::Definition
{

	// 遷移の定義
	template<typename StateType>
	struct TransitionDefinition
	{
		StateType from;// 遷移元の状態
		StateType to;// 遷移先の状態
		std::unique_ptr<IConditrion> condition;// 遷移条件 (RuntimeContextを使って評価)

		int priority = 0; // 遷移の優先度
		std::string tag; // 任意の分類・カテゴリなど
	};

	// 状態の定義
	template<typename StateType, typename ExtensionType = std::monostate>
	struct StateDefinition
	{
		StateType state;// 状態の識別子

		ExtensionType extension{}; // 状態に関連する拡張情報（オプション）
	};

	// FSMの基盤の定義
	template<typename StateType, typename ExtensionType = std::monostate>
	struct FSMDefinition
	{
		StateType initialState;// 初期状態
		std::vector<StateDefinition<StateType, ExtensionType>> states;// 状態の定義のリスト
		std::vector<TransitionDefinition<StateType>> transitions;// 遷移の定義のリスト
	};
}