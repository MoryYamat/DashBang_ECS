#include "Engine/FSM/Public/Base/FSMApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/FSM/Public/Base/Registry.hpp"

#include <iostream>

namespace Engine::FSM::Base
{
	void InitFSMEngine(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		if (!ctx.ww.HasResource<FSMRegistry>())
			ctx.ww.CreateResource<FSMRegistry>();
		if (!ctx.ww.HasResource<FSMCatalog>())
			ctx.ww.CreateResource<FSMCatalog>();
		if (!ctx.ww.HasResource<AxisRuntimeDB>())
			ctx.ww.CreateResource<AxisRuntimeDB>();

	}
	// 2) DTO 登録はここ**だけ**
	void RegisterAxes(Engine::WorldSystem::Core::WorldCtx& ctx, RegisterFn fn)
	{
		auto& reg = ctx.ww.GetResource<FSMRegistry>();
		reg = FSMRegistry{};          // クリーンに
		if (fn) fn(reg);              // ゲーム側で reg.add(AxisDTO/FSMDTO) を行う
	}

	bool BuildFSMCatalog(Engine::WorldSystem::Core::WorldCtx& ctx
		, BuildStrictness policy)
	{
		auto& reg = ctx.ww.GetResource<FSMRegistry>();
		auto& cat = ctx.ww.GetResource<FSMCatalog>();
		BuildResult res = reg.build(policy);

		auto& db = ctx.ww.GetResource<AxisRuntimeDB>();

		if (!res.ok())
		{
			for (const auto& m : res.errors.msgs)
			{
				std::printf("ERR: %s\n", m.c_str());
			}
			cat = FSMCatalog{};
			db = AxisRuntimeDB{};
			return false;
		}

		cat = std::move(res.catalog);

		//
		db = AxisRuntimeDB{};
		for (auto& ax : cat.axes)
		{
			db.ensure(ax);
		}

		for (auto& ax : cat.axes) {
			std::printf("Axis '%s' condOrder:\n", ax.axisName.c_str());
			for (auto& c : ax.condOrder)
				std::printf("  - %s\n", c.c_str());
		}


		return true;
	}

	void InitAxisRuntime(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& cat = ctx.ww.GetResource<FSMCatalog>();
		auto& db = ctx.ww.CreateResource<AxisRuntimeDB>();

		for (const auto& ax : cat.axes)
		{
			AxisRuntime& rt = db.ensure(ax);
			rt.conds.init(ax.condOrder.size());
			// 
		}
	}


	// 条件関数バインド
	bool BindAxisConds
	(
		Engine::WorldSystem::Core::WorldCtx& ctx,
		CondProviderFn provider
	)
	{
		auto& cat = ctx.ww.GetResource<FSMCatalog>();
		auto& db = ctx.ww.GetResource<AxisRuntimeDB>();

		if (!provider) return true;
		std::vector<AxisCondBindings> list;
		provider(cat, list);

		bool all_ok = true;
		for (auto& x : list) {
			AxisRuntime* rt = db.get(x.axisName);
			if (!rt || !rt->canon) {
				std::printf("Axis '%s' not found for cond bindings\n", x.axisName.c_str());
				all_ok = false;
				continue;
			}
			// ここで span は**動的長**。固定長(span<T,N>)に(ptr,size)渡しは厳禁！
			if (!SetupAxisRuntimeConds(*rt, *rt->canon, x.bindings)) {
				all_ok = false;
			}
		}
		return all_ok;

	}

	bool InitAllFSMs(Engine::WorldSystem::Core::WorldCtx& ctx,
		RegisterFn registerFn,
		CondProviderFn condProvider,
		BuildStrictness policy)
	{
		InitFSMEngine(ctx);
		RegisterAxes(ctx, registerFn);
		if (!BuildFSMCatalog(ctx, policy)) return false;
		return BindAxisConds(ctx, condProvider);

	}

}