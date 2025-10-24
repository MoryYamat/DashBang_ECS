#pragma once

#include "Game/Character/Private/FSM/CC/StateModel/CCFSMStates.hpp"

#include <typeindex>

namespace Game::Character::FSM::CC::AxisTag
{
	inline const std::type_index CCAxis = typeid(Game::Character::FSM::CC::StateModel::CCAxis);
}

namespace Game::Character::FSM::CC::StateTag
{

	inline const std::type_index NONE = typeid(Game::Character::FSM::CC::StateModel::CCState::None);
	inline const std::type_index STUNNED = typeid(Game::Character::FSM::CC::StateModel::CCState::Stunned);
	inline const std::type_index KNOCKDOWNED = typeid(Game::Character::FSM::CC::StateModel::CCState::Knockdowned);
	inline const std::type_index IMMUNE = typeid(Game::Character::FSM::CC::StateModel::CCState::Immune);
}