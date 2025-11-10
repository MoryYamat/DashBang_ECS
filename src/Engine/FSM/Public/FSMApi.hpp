#pragma once

#include "Engine/FSM/Public/Core/Registry.hpp"
#include "Engine/Public/EngineFwd.hpp"


#include <span>
#include <string>

namespace Engine::FSM::Core
{
	using RegisterFn = void(*)(FSMRegistry& reg);  // DTO を reg.add(...) する

	void InitFSMEngine(Engine::WorldSystem::Core::WorldCtx& ctx);
	void RegisterAxes(Engine::WorldSystem::Core::WorldCtx& ctx, RegisterFn fn);   // 2)

	bool BuildFSMCatalog(Engine::WorldSystem::Core::WorldCtx& ctx, BuildStrictness policy = BuildStrictness::Lenient);

	// 便利用：全部やる
	bool InitAllFSMs(Engine::WorldSystem::Core::WorldCtx& ctx,
		RegisterFn registerFn,
		BuildStrictness policy = BuildStrictness::Strict);

}