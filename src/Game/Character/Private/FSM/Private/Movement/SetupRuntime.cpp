#include "Game/Character/Private/FSM/Public/FSMApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/FSM/Public/Base/Types.hpp"

#include "Engine/FSM/Public/Base/FSMApi.hpp"

namespace Game::Character::FSM::Movement
{
	using namespace Engine::FSM::Base;

	void SetupMovementAxisRuntime(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& cat = ctx.ww.GetResource<FSMCatalog>();
		auto& db = ctx.ww.GetResource<AxisRuntimeDB>();

		//
		const CanonicalAxis* ax = nullptr;
		for(const auto& a: cat.axes)
			if (a.axisName == "Movement")
			{
				ax = &a;
				break;
			}
		if (!ax) return;

		//
		AxisRuntime& rt = db.ensure(*ax);
		rt.conds.init(ax->condOrder.size());

		//
		//[[maybe_unused]] bool ok = SetupAxisRuntimeConds(rt, *ax, MovementCondBindings);
	}
}