#pragma once

#include "Game/Combat/Private/Skill/FSM/SkillStateTags.hpp"

#include <cstdint>
#include <typeindex>

namespace Game::Character::Animation::Query
{
	struct SkillAnimQueryComponent
	{
		bool isSkillActive = false;
		std::type_index currentPhase = Game::Combat::Skill::FSM::StateTag::NONE;

		uint32_t skillId = 0;

		float elapsedTime = 0.0f;
		float phaseElapsedTime = 0.0f;
		float normalizedTime = 0.0f;

		bool isInterrupted = false;

		// Marker 時間境界
		// float castingEnd = 0.0f;
		// float activeEnd = 0.0f;
		// float recoveryEnd = 0.0f;

		bool valid = false;
	};
}