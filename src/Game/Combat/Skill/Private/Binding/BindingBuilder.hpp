#pragma once

#include "Engine/FSM/Public/FSMFwd.hpp"

#include "Game/Combat/HitBox/Public/HitboxFwd.hpp"

#include "Game/Combat/Skill/Public/Binding/DTO.hpp"
#include "Game/Combat/Skill/Public/Binding/Types.hpp"

#include <string>
#include <vector>
#include <optional>

namespace Game::Combat::Skill::Binding
{
	struct SkillBindingBuildErrors
	{
		std::vector<std::string> msgs;
		bool ok() const { return msgs.empty(); }
		void err(std::string m) { msgs.emplace_back(std::move(m)); }
	};

	struct SkillBindingBuildResult
	{
		SkillBindingData data;
		SkillBindingBuildErrors errs;
	};

	class SkillBindingBuilder
	{
	public:
		// catalog と SkillAxis のID を渡し作成
		SkillBindingBuilder(
			const SkillCatalog& skillCat,
			const HitBox::HitBoxCatalog& hitBoxCat,
			const Engine::FSM::Core::FSMCatalog& fsmCat,
			Engine::FSM::Core::AxisID skillAxis
		);

		void Add(const SkillHitBoxBindingDTO& dto);

		void AddRange(const std::vector<SkillHitBoxBindingDTO>& dtos);

		// まとめてビルド
		SkillBindingBuildResult Build() const;

	private:
		const SkillCatalog& skills_;
		const HitBox::HitBoxCatalog& hitboxes_;
		const Engine::FSM::Core::FSMCatalog& fsm_;
		Engine::FSM::Core::AxisID skillAxis_;

		std::vector<SkillHitBoxBindingDTO> hbdefs_;

		bool buildOne(
			const SkillHitBoxBindingDTO& hbdtos,
			SkillBindingData& out,
			SkillBindingBuildErrors& errs
		) const;

		std::optional<Engine::FSM::Core::StateID>
			ResolveState(
				std::string_view stateName,
				SkillBindingBuildErrors& errs,
				const SkillHitBoxBindingDTO& dto) const;

	};
}