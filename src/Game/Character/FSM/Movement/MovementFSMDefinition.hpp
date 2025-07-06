#pragma once


#include "Game/Common/Logic/FSM/FSMCommon.hpp"

#include "Common/GameNamespaceDecl.h"

namespace Game::Character::FSM::Movement
{
	namespace MovementState
	{
		struct Idle : gNsCommonLogicFSM::IFSMStateTag {};
	}
}