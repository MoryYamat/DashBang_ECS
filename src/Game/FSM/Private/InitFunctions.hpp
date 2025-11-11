#pragma once

#include "Engine/FSM/Public/FSMApi.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"

#include "Game/Character/Private/FSM/Public/FSMApi.hpp"

#include <string_view>

namespace Game::FSM
{
	//inline Engine::FSM::Core::RegisterFn MakeGameRegisterProvider()
	//{
	//	return [](FSMRegistry& reg)
	//		{
	//			Game::Character::FSM::Movement::RegisterMovementAxes(reg);
	//		};
	//}

	inline Engine::FSM::Core::FieldResolverProvider MakeGameResolverProvider()
	{
		return [](std::string_view axisName, Engine::FSM::Core::FieldResolver& out)->bool
			{
				if (axisName == "Movement") { out = Game::Character::FSM::Movement::MakeMovementFieldResolver(); return true; }
				// 他 Axis はここに追加("Skill" など)
				return false;// 未登録Axis
			};
	}
}