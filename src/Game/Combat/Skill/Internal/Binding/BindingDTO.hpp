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

	// Animation 再生区間
	struct PlaySection
	{
		// -1 は 未設定
		float start = -1.0f;
		float end = -1.0f;
	};

	// Animation 用 DTO
	struct SkillAnimationBindingDTO
	{
		SkillStateKeyDTO key;
		std::string anim;		// AnimDTO.name
		// std::string layer;
		PlaySection section;
		bool loop = false;
		float speed = 1.0f;
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