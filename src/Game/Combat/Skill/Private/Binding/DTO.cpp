#include "Game/Combat/Skill/Internal/Binding/BindingDTO.hpp"

#include "Game/Combat/Skill/Private/Binding/BindingBuilder.hpp"

#include "Game/Combat/Skill/Internal/Binding/BindingTypes.hpp"

namespace
{
	using namespace Game::Combat::Skill::Binding;
	void make_BindingDTO(SkillBindingBuilder& builder)
	{
		SkillHitBoxBindingDTO test_skill_hb;
		test_skill_hb.key.skill = "test_skill";
		test_skill_hb.key.state = "Active";
		test_skill_hb.hitbox = "default_obb2D";
		SkillAnimationBindingDTO test_skill_anim;
		test_skill_anim.key.skill = "test_skill";
		test_skill_anim.key.state = "Active";
		test_skill_anim.anim = "slash_outward_default";
		test_skill_anim.loop = false;
		test_skill_anim.speed = 1.0f;
		test_skill_anim.startTime = 0.0f;
		test_skill_anim.endTime = -1.0f;
		test_skill_anim.loopWithinRange = false;

		builder.Add(test_skill_hb);
		builder.Add_Anim(test_skill_anim);
	}
}

namespace Game::Combat::Skill::Binding
{
	void RegisterSkillBindingDTO(Game::Combat::Skill::Binding::SkillBindingBuilder& builder)
	{
		make_BindingDTO(builder);
	}
}