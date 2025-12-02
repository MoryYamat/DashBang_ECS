#include "BindingBuilder.hpp"

#include "Game/Combat/Skill/Public/Binding/Types.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"
#include "Engine/FSM/Public/Core/ID-NameHelper.hpp"

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
		const Engine::FSM::Core::FSMCatalog& fsmCat,
		Engine::FSM::Core::AxisID skillAxis
	) : skills_(skillCat)
		, hitboxes_(hitBoxCat)
		, fsm_(fsmCat)
		, skillAxis_(skillAxis)
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

		for (const auto& dto : hbdefs_)
		{
			buildOne(dto, result.data, result.errs);
		}

		return result;
	}

	bool SkillBindingBuilder::buildOne(
		const SkillHitBoxBindingDTO& hbdtos,
		SkillBindingData& hbout,
		SkillBindingBuildErrors& errs
	) const
	{
		bool ok = true;

		SkillID skillId{};
		// Skill名 → SkillID
		{

		}

		// State名 → StateID (Skill軸のFSMから)
		auto stateIdOpt = ResolveState(hbdtos.key.state, errs, hbdtos);
		{
			if (!stateIdOpt)
			{
				return false;
			}
		}
		StateID stateId = *stateIdOpt;

		// HitBox名 → HitBoxID
		HitBox::HitBoxID hitBoxId{};
		{
			
		}

		// 正常に解決できたらエントリ追加
		SkillHitBoxBindingEntry hbentry;
		hbentry.skill = skillId;
		hbentry.state = stateId;
		hbentry.hitbox = hitBoxId;

		hbout.entries.push_back(hbentry);

		return ok;
	}


	std::optional<Engine::FSM::Core::StateID>
		SkillBindingBuilder::ResolveState(
			std::string_view stateName,
			SkillBindingBuildErrors& errs,
			const SkillHitBoxBindingDTO& dto) const
	{

		// 
		const auto& axis = fsm_.axes[skillAxis_.v];
		std::uint32_t s_id = Engine::FSM::Core::AxisLookup::FindStateIdx(axis, stateName);
		std::optional<StateID> id = StateID(s_id);

		return id;
	}
}