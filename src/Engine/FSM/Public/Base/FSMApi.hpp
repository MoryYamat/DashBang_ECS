#pragma once

#include "Engine/FSM/Public/Base/Registry.hpp"
#include "Engine/Public/EngineFwd.hpp"

#include <span>
#include <string>

namespace Engine::FSM::Base
{
	using RegisterFn = void(*)(FSMRegistry& reg);  // DTO を reg.add(...) する
	struct AxisCondBindings { std::string axisName; std::span<const NamedCondBinding> bindings; };
	using CondProviderFn = void(*)(const FSMCatalog& cat, std::vector<AxisCondBindings>& out);

	void InitFSMEngine(Engine::WorldSystem::Core::WorldCtx& ctx);
	void RegisterAxes(Engine::WorldSystem::Core::WorldCtx& ctx, RegisterFn fn);   // 2)

	bool BuildFSMCatalog(Engine::WorldSystem::Core::WorldCtx& ctx, BuildStrictness policy = BuildStrictness::Lenient);

	bool SetupAxisRuntimeConds
	(
		AxisRuntime& rt,
		const CanonicalAxis& ax,
		std::span<const NamedCondBinding> bindings
	);

	bool BindAxisConds
	(
		Engine::WorldSystem::Core::WorldCtx& ctx, 
		CondProviderFn provider
	);

	// 便利用：全部やる
	bool InitAllFSMs(Engine::WorldSystem::Core::WorldCtx& ctx,
		RegisterFn registerFn,
		CondProviderFn condProvider,
		BuildStrictness policy = BuildStrictness::Strict);

}