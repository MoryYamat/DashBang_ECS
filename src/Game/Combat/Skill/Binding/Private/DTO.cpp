#include "Game/Combat/Skill/Binding/Public/DTO.hpp"

#include "Game/Combat/Skill/Binding/Private/BindingBuilder.hpp"

#include "Game/Combat/Skill/Binding/Public/Types.hpp"

namespace
{
	using namespace Game::Combat::Skill::Binding;
	void make_BindingDTO(SkillBindingBuilder& builder)
	{
		SkillHitBoxBindingDTO test_skill_hb;
		test_skill_hb.key.skill = "test_skill";
		test_skill_hb.key.state = "Active";
		test_skill_hb.hitbox = "default_obb2D";

		builder.Add(test_skill_hb);
	}
}

namespace Game::Combat::Skill::Binding
{
	void RegisterSkillBindingDTO(Game::Combat::Skill::Binding::SkillBindingBuilder& builder)
	{
		make_BindingDTO(builder);
	}
}