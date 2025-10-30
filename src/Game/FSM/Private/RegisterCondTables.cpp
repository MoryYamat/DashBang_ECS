#include "Game/FSM/Public/FSMApi.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"

#include "Game/Character/Private/FSM/Public/FSMApi.hpp"

#include <string>
#include <unordered_map>

namespace Game::FSM
{
	using namespace Engine::FSM::Core;

	std::unordered_map<std::string, CondTableStaging> CollectAllCondStages()
	{
		std::unordered_map<std::string, CondTableStaging> map;

		// movement
		{
			CondTableStaging st;
			Game::Character::FSM::Movement::BuildMovementCondTable_Stage(st);
			map.emplace("Movement", std::move(st));
		}

		return map;
	}

	Engine::FSM::Core::CondStagesPerAxisProfile CollectAllCondStagesWithProfiles()
	{
		CondStagesPerAxisProfile map;

		// movement / default(0)
		{
			CondTableStaging st;
			Game::Character::FSM::Movement::BuildMovementCondTable_Stage(st);
			map.emplace(AxisProfileKey{ "Movement", 0 }, std::move(st));
		}

		// 
		{

		}

		return map;
	}
}