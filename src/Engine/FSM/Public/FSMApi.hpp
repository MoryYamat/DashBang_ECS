#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"

#include <unordered_map>
#include <string>

namespace Engine::FSM::Core
{
	void InitFSMEngine(Engine::WorldSystem::Core::WorldCtx& ctx);

	void InitFSMRegistry(Engine::WorldSystem::Core::WorldCtx& ctx);

	void InitFSMCatalog(Engine::WorldSystem::Core::WorldCtx& ctx);

	// build後のCanonicalFSM → 各システムはこれを利用
	void BuildCanonicalFSM(Engine::WorldSystem::Core::WorldCtx& ctx);

	// InitFSMEngine();
	// RegisterAllAxes();
	// BuildCanonicalFSM();


	// Cond
	void InitFSMCondTables(Engine::WorldSystem::Core::WorldCtx& ctx);
	void BuildCondTables(Engine::WorldSystem::Core::WorldCtx& ctx,
						const std::unordered_map<std::string, CondTableStaging>& perAxis);



}