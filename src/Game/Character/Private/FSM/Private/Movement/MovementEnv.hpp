#pragma once

#include "Engine/FSM/Public/Core/Types.hpp"

namespace Game::Character::FSM::Movement
{
	struct MovementEnv : Engine::FSM::Core::EnvSnapshot
	{
		bool canMove = true;// CCなどによって移動不可能な場合はfalse
		bool wantMove = false;// 入力意図により移動しようとしているかどうか
	};
}

//Game/Character/Private/FSM/Movement/
//├─ MovementEnv.hpp
//├─ MovementEnvBuilder.hpp
//├─ MovementEnvBuilder.cpp
//├─ MovementCondFactory.hpp
//├─ MovementCondFactory.cpp
//└─ MovementSelectorSystem.cpp   // 実行サンプル（更新ループ側）