#pragma once

#include "Engine/ECS/Entity.h"

#include "Game/Character/FSM/Movement/MovementStateTags.hpp"

#include "Game/Character/FSM/Interference/Core/Data/FSMInterferenceRequest.hpp"

#include <typeindex>
#include <optional>
#include <any>

namespace Game::Character::FSM::Movement::StateModel
{

	struct MovementFSMLeaseComponent
	{
		std::type_index issuerAxis = Game::Character::FSM::Movement::AxisTag::MovementAxis;// 干渉元のFSM識別子
		Engine::ECS::Entity issuerEntity;// 干渉元エンティティ(例: CCエフェクトを与えた相手)

		Game::Character::FSM::Interference::Core::Data::ControlSeverity severity;// 干渉の優先度比較
		Game::Character::FSM::Interference::Core::Data::InterferenceMode mode;

		std::optional<std::type_index> forcedState;// 強制遷移先の状態型

		float remainingDurationSec = 0.0f;// 残り干渉時間

		// std::any customContext;// 拡張情報


		inline void reset()
		{
			issuerAxis = Game::Character::FSM::Movement::AxisTag::MovementAxis;
			// issuerEntity = 

			forcedState = std::nullopt;
			remainingDurationSec = 0.0f;
			severity = Game::Character::FSM::Interference::Core::Data::ControlSeverity::Low;
			mode = Game::Character::FSM::Interference::Core::Data::InterferenceMode::None;
		}

		inline void tick(float dt)
		{
			remainingDurationSec -= dt;
		}

		inline bool hasTimeLeft(float eps)
		{
			return remainingDurationSec - eps >= 0;
		}

		inline const bool isActive(float eps) const
		{
			return (mode != Game::Character::FSM::Interference::Core::Data::InterferenceMode::None)
				&&
				(remainingDurationSec > eps);
		}
	};
}