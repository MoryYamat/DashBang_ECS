#pragma once

#include "Game/Combat/Skill/Core/Public/SkillFwd.hpp"
#include "Game/Combat/HitBox/Public/HitBoxFwd.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"



namespace Game::Combat::Skill::Binding
{
	struct SkillHitBoxBindingEntry
	{
		Game::Combat::Skill::SkillID skill;
		Engine::FSM::Core::StateID state;
		Game::Combat::HitBox::HitBoxID hitbox;
	};

	struct SkillBindingData
	{
		// std::vector<std::uint16_t> offs;

		std::vector<SkillHitBoxBindingEntry> hbs;// 必要ならO(1). unordered_map or csr
		// 将来はほかの効果も追加していく

		// 将来的にはunordered_map / CSR 
		Game::Combat::HitBox::HitBoxID resolveHitBox(
			Game::Combat::Skill::SkillID skill,
			Engine::FSM::Core::StateID state
		) const
		{
			for (const auto& e : hbs)
			{
				if (e.skill == skill && e.state == state)
				{
					return e.hitbox;
				}
			}
			return Game::Combat::HitBox::kInvalidHitBoxID;
		}
	};


}