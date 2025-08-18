#pragma once

#include "Engine/ECS/Entity.h"

#include "Game/Character/FSM/Interference/Core/Data/FSMInterferenceRequest.hpp"

#include <typeindex>
#include <optional>
#include <any>

namespace Game::Character::FSM::Movement::StateModel
{
	using namespace Engine::ECS;
	using namespace Game::Character::FSM::Interference::Core::Data;

	struct MovementFSMLeaseComponent
	{
		std::type_index issuerAxis;// 干渉元のFSM識別子
		Entity issuerEntity;// 干渉元エンティティ(例: CCエフェクトを与えた相手)

		ControlSeverity severity;// 干渉の優先度比較
		InterferenceMode mode;

		std::optional<std::type_index> forocedState;// 強制遷移先の状態型

		float remainingDurationSec = 0.0f;// 残り干渉時間
		std::any customContext;// 拡張情報
	};
}