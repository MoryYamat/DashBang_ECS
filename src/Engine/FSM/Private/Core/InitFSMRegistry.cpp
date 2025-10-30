#include "Engine/FSM/Public/FSMApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/FSM/Public/Core/Registry.hpp"
#include "Engine/FSM/Public/Core/Types.hpp"

namespace Engine::FSM::Core
{
	void InitFSMRegistry(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		if (!ctx.ww.HasResource<FSMRegistry>())
		{
			ctx.ww.CreateResource<FSMRegistry>();
		}
	}


	void InitFSMCatalog(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		if (!ctx.ww.HasResource<FSMCatalog>())
		{
			ctx.ww.CreateResource<FSMCatalog>();
		}
	}

	void InitFSMEngine(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		InitFSMRegistry(ctx);
		InitFSMCatalog(ctx);
	}

	void BuildCanonicalFSM(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& reg = ctx.ww.GetResource<FSMRegistry>();
		auto& cat = ctx.ww.GetResource<FSMCatalog>();
		cat.axes = reg.build();
	}


	void InitFSMCondTables(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		if (!ctx.ww.HasResource<FSMCondTables>())
		{
			ctx.ww.CreateResource<FSMCondTables>();
		}
	}

	void BuildCondTables(Engine::WorldSystem::Core::WorldCtx& ctx,
		const std::unordered_map<std::string, CondTableStaging>& perAxis)
	{
		auto& cat = ctx.ww.GetResource<FSMCatalog>();
		auto& set = ctx.ww.GetResource<FSMCondTables>();
		set.byAxis.clear();
		set.byAxis.resize(cat.axes.size());

		for (std::size_t i = 0; i < cat.axes.size(); ++i)
		{
			const auto& ca = cat.axes[i];
			auto it = perAxis.find(ca.axisName);
			if (it == perAxis.end())
			{
				continue;
			}

			FinalizeCondTable(ca, it->second, set.byAxis[i]);
		}
	}

	void InitFSMCondProfiles(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& cat = ctx.ww.GetResource<FSMCatalog>();
		if (!ctx.ww.HasResource<FSMCondProfiles>())
			ctx.ww.CreateResource<FSMCondProfiles>();
		auto& profs = ctx.ww.GetResource<FSMCondProfiles>();
		profs.byAxis.clear();
		profs.byAxis.resize(cat.axes.size());
	}


	void BuildCondProfiles(Engine::WorldSystem::Core::WorldCtx& ctx,
		const CondStagesPerAxisProfile& perAxisProfile)
	{
		auto& cat = ctx.ww.GetResource<FSMCatalog>();
		auto& profs = ctx.ww.GetResource<FSMCondProfiles>();

		std::unordered_map<std::string, std::size_t> axisIndex;
		axisIndex.reserve(cat.axes.size());
		for (std::size_t i = 0; i < cat.axes.size(); ++i)
		{
			axisIndex.emplace(cat.axes[i].axisName, i);
		}

		for (const auto& [key, stage] : perAxisProfile)
		{
			auto itAx = axisIndex.find(key.axisName);
			if (itAx == axisIndex.end()) continue;
			auto ax = itAx->second;
			CondTable& dst = profs.byAxis[ax][key.profileId];
			FinalizeCondTable(cat.axes[ax], stage, dst);
		}

		for (std::size_t ax = 0; ax < cat.axes.size(); ++ax)
		{
			auto& perAxis = profs.byAxis[ax];
			if (!perAxis.count(0)) perAxis.emplace(0, CondTable{});
		}
	}
}