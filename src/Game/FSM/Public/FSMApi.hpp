#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/FSM/Public/Core/Types.hpp"

#include <unordered_map>
#include <string>

namespace Game::FSM
{
	// Init AllFSMs
	void InitAllFSMs(Engine::WorldSystem::Core::WorldCtx& ctx);


	// 
	void RegisterAllAxes(Engine::WorldSystem::Core::WorldCtx& ctx);

	std::unordered_map<std::string, Engine::FSM::Core::CondTableStaging> CollectAllCondStages();


	Engine::FSM::Core::CondStagesPerAxisProfile CollectAllCondStagesWithProfiles();

}