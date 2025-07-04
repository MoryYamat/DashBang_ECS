// FSMDefinitionに基づいて実行中の状態を保持・制御する実態
// いつどこに遷移するかを管理する

#pragma once

#include "Game/Common/Logic/FSM/FSMDefinition.hpp"
#include "Game/COmmon/Logic/FSM/Condition/RuntimeContext.hpp"

#include <vector>
#include <variant>


namespace Game::Common::Logic::FSM::Runtime
{
	// FSMの実行インスタンス
	template<typename StateType, typename ExtensionType = std::monostate>
	class FSMInstance
	{
	public:
		using Definition = FSMDefinition<StateType, ExtensionType>;
		using Transition = TransitionDefinition<StateType>;

		// コンストラクタ
		FSMInstance(const Definition& def)
			: Definition(def), currentState(def.initialState), elapsedInState(0.0f) { }

		void update(const RuntimeContext& ctx, float deltaTime)
		{
			elapsedInState += deltaTime; // 現在の状態にいる時間を更新

			// すべての遷移候補を取得
			std::vector<Transition*> candidates;
			for (const auto& t : definition.transitions)
			{
				if (t.from == currentState && t.condition->evaluate(ctx))
				{
					candidates.push_back(&t);
				}
			}

			// 条件を満たす遷移がある場合、優先度の高いものを選択
			if(!candidates.empty())
			{
				// 優先度でソート
				std::sort(candidates.begin(), candidates.end(),
					[](const auto* a, const auto* b) { return a->priority > b->priority;});

				// 最も優先度の高い遷移に遷移
				transitionTo(candidates.front()->to); // 最も優先度の高い遷移に遷移
			}
		}

		StateType getState() const { return currentState; }

	private:
		// 状態遷移の実行
		void transitionTo(StateType nextState)
		{
			// 遷移先の状態に遷移する
			currentState = nextState;
			elapsedInState = 0.0f; // 状態遷移時に時間をリセット
		}

		const Definition& definition;// FSMの定義
		StateType currentState; // 現在の状態
		float elapsedInState; // 現在の状態にいる時間
	};
}