// 「スキル定義に基づいた状態遷移の進行」

#pragma once 

#include "Engine/ECS/Entity.h"

#include "Common/EngineNamespaceDecl.h"

#include "Game/Combat/Skill/FSM/SkillStateTags.hpp"

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <typeindex>

// TODO: マジックナンバーをすべて修正する
namespace Game::Combat::Skill::Component
{
	// スキルの実行フェーズを定義する列挙型
	enum class SkillExecutionPhase
	{
		Casting,// スキルのキャスト段階
		Active, // スキルのアクティブ段階(攻撃など)
		Recovery,// スキルの硬直
		Completed, // スキルの完了段階(終了状態)
		Interrupted, // スキルの中断段階(スタンや死亡など)
		Canceled, // スキルキャンセル
	};

	// スキルの実行フェーズイベントを定義する列挙型
	enum class SkillExecutionEvent
	{
		OnEneterCasting, // スキルのキャスト開始
		OnExitCasting, // スキルのキャスト終了
		OnEnterActive, // スキルのアクティブ開始
		OnExitActive, // スキルのアクティブ終了
		OnEnterRecovery, // スキルのリカバリー開始
		OnExitRecovery, // スキルのリカバリー終了
		OnCompleted, // スキルの完了
		OnInterrupted, // スキルの中断
		OnCanceled, // スキルのキャンセル
	};

	using namespace Game::Combat::Skill::FSM;

	// スキル実行状態メタ情報
	struct SkillExecutionContextComponent
	{
		eNsECS::Entity caster = eNsECS::Entity::INVALID;
		uint32_t skillId = 0;

		float elapsedTime = 0.0f;
		float phaseElapsedTime = 0.0f;
		bool isInterrupted = false;

		std::type_index previousState = StateTag::NONE;
	};
}