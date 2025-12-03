// 正規化 / 前計算

#pragma once
#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"
#include "Game/Combat/Skill/Public/SkillFwd.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <string_view>
#include <string>
#include <unordered_map>
#include <cstdint>

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
		float timeOffset = 0.f;
		float lifetime = 0.f;
		EffectKind kind = EffectKind::SpawnHitBox;
		//EffectSlotID slot;			// 今は同じ種類のEffectは一つだけだと仮定する // 同じ種類のEffectを複数使い分けるためのID 例) 0=main, 1=sub, ...
	};

	struct CanonicalSkillData
	{
		std::string name;			// skill name
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

		std::unordered_map<std::string, SkillID> nameToId;

		SkillID FindByName(std::string_view name)const
		{
			auto it = nameToId.find(std::string(name));
			if (it == nameToId.end())
			{
				return kInvalidSkillID;
			}
			return it->second;
		}
	};

	// intent component
	struct SkillRequest
	{
		Game::Combat::Skill::SkillID skill;
		Engine::ECS::Core::Entity caster;
		glm::vec2 targetPos;
		Engine::ECS::Core::Entity targetEntity;
		bool isQueued;
	};

	// 「Charancter」の Skill 実行にかかわる「状態」を保持 (Hitbox や Anim の具体的な状態は持たない)
	// 実行状態
	struct SkillRuntimeComp
	{
		SkillID curSkill = kInvalidSkillID;
		const CanonicalSkillData* curSkillData = nullptr;
		bool isActive = false;

		Engine::FSM::Core::StateID state = Engine::FSM::Core::kInvalidState;		// casting/active/recovery etc.
		float totalElapsed = 0.f;
		float phaseElapsed = 0.f;
		float prevPhaseElapsed = 0.f;

		// 
		bool acceptedTriggerThisFrame = false;
		float curPhaseDuration = 0.f;
		bool hasPhaseDuration = false;

		// 外部入力
		bool triggerRequested = false;
		bool cancelRequested = false;

		std::vector<SkillRequest> pendingTriggers;



		void ResetInput()
		{
			triggerRequested	= false;
			cancelRequested		= false;
			pendingTriggers.clear();
		}

		void ResetTimeElapsed()
		{
			totalElapsed = 0.f;
			phaseElapsed = 0.f;
			prevPhaseElapsed = 0.f;
		}
	};


}


// SkillCatalogはグローバルリソースで保持
// SkillRuntimeComp は各Entityが持つ
// Intent->SkillIDのマッピングは入力系で行う
//