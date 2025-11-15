#pragma once

#include "Engine/FSM/Public/FSMApi.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"

#include "Game/Character/Private/FSM/Public/FSMApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Character/Private/FSM/Public/MovementAxisTypes.hpp"
#include "Game/Character/Private/FSM/Public/MovementAxisApi.hpp"

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

	inline bool InitGameCondTable(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		using namespace Game::Character::FSM::Movement;

		bool ok = true;

		{
			auto& tbl = ctx.ww.CreateResource<MovementCondTable>();
			if (!InitMovementCondTable(ctx, tbl))
			{
				std::printf("[InitGameCondTable] Failed to init MovementCondTable\n");
				ok = false;
			}
		}

		// 以下にほかの軸も追加していく


		return ok;
	}
}