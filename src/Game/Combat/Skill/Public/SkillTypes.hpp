// 正規化 / 前計算

#pragma once

#include "Engine/FSM/Public/FSMFwd.hpp"

#include "SkillFwd.hpp"

#include <vector>

namespace Game::Combat::Skill
{
	
	struct CanonicalSkillPhase
	{
		Engine::FSM::Core::StateID state;		// Skill軸の一意なStateIDを流用
		float duration;
	};

	enum class EffectKind : std::uint8_t
	{
		SpawnHitBox,
		PlayAnim,
		// SoundやCameraShakeなど
	};

	struct CanonicalSkillEffect
	{
		Engine::FSM::Core::StateID state;		// どのフェーズか
		float timeOffset;
		EffectKind kind;
		//EffectSlotID slot;			// 今は同じ種類のEffectは一つだけだと仮定する // 同じ種類のEffectを複数使い分けるためのID 例) 0=main, 1=sub, ...
	};

	struct CanonicalSkillData
	{
		SkillID id;					// skillDefDTO.name の定義順に0から採番
		std::vector<CanonicalSkillPhase> phases;
		std::vector<CanonicalSkillEffect> effects;

		// std::vector<std::string> skillIdxByName;

		// FSMとの紐づけ
		Engine::FSM::Core::AxisID axis;			// "Skill"確定
		Engine::FSM::Core::FSMID fsm;			// "Basic"のIDを基本として、遷移構造としての識別子
		Engine::FSM::Core::ProfileID profile;	// "Skill_211"のためのProfileなど
	};

	struct SkillCatalog
	{
		std::vector<CanonicalSkillData> skills;
	};

	// 実行状態
	struct SkillRuntimeComp
	{
		SkillID curSkill = kInvalidSkillID;
		const CanonicalSkillData* curSkillData = nullptr;
		bool isActive = false;

		Engine::FSM::Core::StateID state = Engine::FSM::Core::kInvalidState;		// casting/active/recovery etc.
		float totalElapsed = 0.f;
		float phaseElapsed = 0.f;

		// 外部入力
		bool triggerRequested = false;
		bool cancelRequested = false;

		void resetInput()
		{
			triggerRequested	= false;
			cancelRequested		= false;
		}
	};

}


// SkillCatalogはグローバルリソースで保持
// SkillRuntimeComp は各Entityが持つ
// Intent->SkillIDのマッピングは入力系で行う
//