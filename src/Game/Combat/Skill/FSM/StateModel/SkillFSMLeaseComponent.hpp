#pragma once

#include "Game/Character/FSM/Interference/Core/Data/FSMInterferenceRequest.hpp"

#include "Game/Combat/Skill/FSM/SkillStateTags.hpp"

namespace Game::Combat::Skill::FSM::StateModel
{
	struct SkillFSMLeaseComponent
	{

		std::type_index issuerAxis;// 発行者FSM識別子
		Engine::ECS::Entity issuerEntity;// 干渉元エンティティ(例: CCエフェクトを与えた相手)

		Game::Character::FSM::Interference::Core::Data::ControlSeverity severity;


		Game::Character::FSM::Interference::Core::Data::InterferenceMode mode;
		std::optional<std::type_index> forcedState; // 強制遷移させたい状態

		float remainingDurationSec = 0.0f;// 最初干渉時間


		inline void reset()
		{
			namespace AxisTag = Game::Combat::Skill::FSM::AxisTag;
			namespace StateTag = Game::Combat::Skill::FSM::StateTag;
			namespace IF = Game::Character::FSM::Interference::Core::Data;

			issuerAxis = AxisTag::SKILLAXIS;
			// issuerEntity =

			severity = IF::ControlSeverity::Low;
			mode = IF::InterferenceMode::None;
			forcedState = std::nullopt;

			remainingDurationSec = 0.0f;
		}

		inline void tick(float dt)
		{
			remainingDurationSec -= dt;
		}

		inline bool hasTimeLeft(float eps) const
		{
			return remainingDurationSec > eps;
		}

		inline bool isActive(float eps) const
		{
			return (mode != Game::Character::FSM::Interference::Core::Data::InterferenceMode::None)
				&& (remainingDurationSec > eps);
		}
	};
}