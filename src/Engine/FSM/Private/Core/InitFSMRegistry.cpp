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


}