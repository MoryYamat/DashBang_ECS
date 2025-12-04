#pragma once


#include <string>

namespace Game::Combat::Skill::Binding
{
	// TODO: 効果チャネル化/効果配列化 etc.
	// キー
	struct SkillStateKeyDTO
	{
		std::string skill;		// SkillDefDTO.name
		std::string state;		// FSM の state名 ("Casting","Active",...)
	};

	// HitBox 用 DTO
	struct SkillHitBoxBindingDTO
	{
		SkillStateKeyDTO key;
		std::string hitbox;		// HitBoxDTO.name
	};

	// Animation 用 DTO
	struct SkillAnimationBindingDTO
	{
		SkillStateKeyDTO key;
		std::string clip;
		std::string layer;
		// loop, speed, etc,...
	};

	// sound 用 DTO
	struct SkillSoundBindingDTO
	{
		SkillStateKeyDTO key;
		std::string cue;
		// volume, 3D/2D, etc...
	};
}