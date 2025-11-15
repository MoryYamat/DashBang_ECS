#include "Game/Character/Private/FSM/Public/MovementAxisApi.hpp"

#include "Game/Character/Private/FSM/Public/MovementAxisComponent.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/InitAxisComponent.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Character::FSM::Movement
{
	using namespace Engine::FSM::Core;
	using namespace Engine::WorldSystem::Core;

	bool InitMovementAxis(WorldCtx& ctx, MovementAxisComp& comp)
	{

		return InitAxisComponent<MovementTag>(ctx, comp);
	}

	bool InitMovementAxis(WorldCtx& ctx, MovementAxisComp& comp, std::string_view fsmName)
	{
		return InitAxisComponent<MovementTag>(ctx, comp, fsmName);
	}

	// Movement軸内の固有cond名の別名をデータベースに登録
	bool InitMovementCondTable(Engine::WorldSystem::Core::WorldCtx& ctx, MovementCondTable& out)
	{
		auto* cat = ctx.ww.TryGetResource<FSMCatalog>();
		if (!cat) return false;

		const CanonicalAxis* movementAxis = nullptr;
		for (auto& ax: cat->axes)
		{
			if (ax.axisName == "Movement")
			{
				movementAxis = &ax;
				break;
			}
		}
		if (!movementAxis)return false;

		const auto& condMap = movementAxis->condIdxByName;

		auto findBit = [&](const char* name, std::uint32_t& dst) -> bool
			{
				auto it = condMap.find(name);
				if (it == condMap.end()) return false;
				dst = it->second;
				return true;
			};


		bool ok = true;

		// ok &= findBit("canMove", out.canMove);
		// ok &= findBit("shouldStop", out.shouldStop);


		return ok;
	}
}