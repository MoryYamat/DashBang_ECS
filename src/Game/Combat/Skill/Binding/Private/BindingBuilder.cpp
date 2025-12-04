#include "BindingBuilder.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/AxisComponent.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"
#include "Engine/FSM/Public/Core/ID-NameHelper.hpp"

#include "Game/Character/FSM/Public/SkillAxisComponent.hpp"

#include "Game/Combat/Skill/Binding/Public/Types.hpp"
#include "Game/Combat/Skill/Core/Public/SkillTypes.hpp"
#include "Game/Combat/HitBox/Public/HitBoxTypes.hpp"

#include "Engine/Log/Public/LogApi.hpp"

#include <string_view>
#include <cstdint>
#include <optional>

namespace
{

}

namespace Game::Combat::Skill::Binding
{
	using namespace Engine::FSM::Core;

	SkillBindingBuilder::SkillBindingBuilder(
		const SkillCatalog& skillCat,
		const HitBox::HitBoxCatalog& hitBoxCat,
		const Engine::FSM::Core::FSMCatalog& fsmCat
	) : skills_(skillCat)
		, hitboxes_(hitBoxCat)
		, fsm_(fsmCat)
	{

	}

	void SkillBindingBuilder::Add(const SkillHitBoxBindingDTO& hitboxdto)
	{
		hbdefs_.push_back(hitboxdto);
	}

	void SkillBindingBuilder::AddRange(const std::vector<SkillHitBoxBindingDTO>& hbdtos)
	{
		hbdefs_.insert(hbdefs_.end(), hbdtos.begin(), hbdtos.end());
	}

	SkillBindingBuildResult SkillBindingBuilder::Build() const
	{
		SkillBindingBuildResult result;

		// Skill FSM Canonical Data の 取得
		std::string_view axisName = AxisTraits<Game::Character::FSM::Skill::SkillTag>::AxisName();
		AxisID skillAxisId = Engine::FSM::Core::AxisLookup::FindAxisIdxByName(fsm_, axisName);
		{
			if (skillAxisId.v >= fsm_.axes.size())
			{
				Engine::Log::Write(Engine::Log::Level::Error,
					"SkillBindingBuilder",
					"Skill axis FSM index out of range.");
				result.errs.err("[SkillBinding] Skill axis FSM index out of range");
				return result;
			}
		}
		const auto& axis = fsm_.axes[skillAxisId.v];

		for (const auto& dto : hbdefs_)
		{
			buildOne(axis, dto, result.data, result.errs);
		}

		return result;
	}

	bool SkillBindingBuilder::buildOne(
		const Engine::FSM::Core::CanonicalAxis& axis,
		const SkillHitBoxBindingDTO& hbdtos,
		SkillBindingData& out,
		SkillBindingBuildErrors& errs
	) const
	{
		
		// Skill名 → SkillID
		SkillID skillId = skills_.FindByName(hbdtos.key.skill);
		{
			if (!skillId.valid())
			{
				errs.err("[SkillBinding] Unknown skill name: '" + hbdtos.key.skill +
					"' (state='" + hbdtos.key.state +
					"' hitbox='" + hbdtos.hitbox + "')");

				return false;
			}
		}

		// State名 → StateID (Skill軸のFSMから)
		std::uint32_t id = Engine::FSM::Core::AxisLookup::FindStateIdx(axis, hbdtos.key.state);
		StateID stateId(id);
		{
			if (!stateId.valid())
			{
				errs.err("[SkillBinding] Unknown state name: '" + hbdtos.key.state +
					"' (skill='" + hbdtos.key.skill +
					"' hitbox='" + hbdtos.hitbox + "')");
				return false;
			}
		}
		
		// HitBox名 → HitBoxID
		HitBox::HitBoxID hitBoxId = hitboxes_.FindByName(hbdtos.hitbox);
		{
			if (!hitBoxId.valid())
			{
				errs.err("[SkillBinding] Unknown HitBox name: '" + hbdtos.key.state+
					"' (state='" + hbdtos.key.skill +
					"' hitbox='" + hbdtos.hitbox + "')");
				return false;
			}
		}


		// TODO: キー拡張によるエラー検出処理変更 (現行仕様ではエラー)
		// ★ (skill, state) の重複チェック
		for (const auto& e : out.hbs)
		{
			if (e.skill == skillId && e.state == stateId)
			{
				errs.err(
					"[SkillBinding] Duplicate binding for "
					"(skill='" + hbdtos.key.skill +
					"', state='" + hbdtos.key.state +
					"') : existingHitBoxId=" + std::to_string(e.hitbox.v) +
					", newHitBox='" + hbdtos.hitbox + "'"
				);
				return false; // 2つ目以降は無視
			}
		}


		// 正常に解決できたらエントリ追加
		SkillHitBoxBindingEntry hbentry;
		hbentry.skill = skillId;
		hbentry.state = stateId;
		hbentry.hitbox = hitBoxId;

		// 追加
		out.hbs.push_back(hbentry);

		return true;
	}
}