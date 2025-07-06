// FSMの基盤の定義

// TODO:
// RuntimeContextの類が必要かどうか設計を見直す必要がある。
// 
// 

#pragma once

#include "Game/Common/Logic/FSM/Condition/IConditrion.hpp"
#include <vector>
#include <optional>
#include <memory>
#include <variant>
#include <string>

#include <tuple>

namespace Game::Common::Logic::FSM::Definition
{
	// FSM状態リスト
	template<typename... States>
	struct StateList
	{
		using Types = std::tuple<States...>;
	};

	// 遷移定義
	template<typename From, typename To, typename Condition>
	struct Transition
	{
		using FromState = From; // 遷移元の状態
		using ToState = To;     // 遷移先の状態
		using ConditionType = Condition; // 遷移条件
	};

	// 遷移リスト
	template<typename... Transitions>
	struct TransitionList
	{
		using Types = std::tuple<Transitions...>;
	};

	// 定義本体
	template<typename Axis, typename InitialState, typename States, typename Transitions>
	struct FSMDefinition
	{
		using AxisType = Axis; // FSMの軸（例: キャラクター、マップなど）
		using InitialStateType = InitialState; // 初期状態の型
		using StateSets = States; // 状態のリスト
		using TransitionSets = Transitions; // 遷移のリスト
	};

	////// 遷移の定義
	//template<typename StateType>
	//struct TransitionDefinition
	//{
	//	StateType from;// 遷移元の状態
	//	StateType to;// 遷移先の状態
	//	std::unique_ptr<IConditrion> condition;// 遷移条件 (RuntimeContextを使って評価)

	//	int priority = 0; // 遷移の優先度
	//	std::string tag; // 任意の分類・カテゴリなど
	//};

	//// 状態の定義
	//template<typename StateType, typename ExtensionType = std::monostate>
	//struct StateDefinition
	//{
	//	StateType state;// 状態の識別子

	//	ExtensionType extension{}; // 状態に関連する拡張情報（オプション）
	//};

	//// FSMの基盤の定義
	//template<typename StateType, typename ExtensionType = std::monostate>
	//struct FSMDefinition
	//{
	//	StateType initialState;// 初期状態
	//	std::vector<StateDefinition<StateType, ExtensionType>> states;// 状態の定義のリスト
	//	std::vector<TransitionDefinition<StateType>> transitions;// 遷移の定義のリスト
	//};
}