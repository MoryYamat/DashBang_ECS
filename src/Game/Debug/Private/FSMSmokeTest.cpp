#include "Game/Debug/Public/DebugApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/FSMApi.hpp"

#include "Game/Character/Private/FSM/Private/Movement/MovementEnv.hpp"

#include <iostream>

namespace Game::FSM::Debug
{
	using namespace Engine::WorldSystem::Core;
	using namespace Engine::FSM::Core;

	void SmokeTest_Movemet(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& cat = ctx.ww.GetResource<FSMCatalog>();
		auto& ctab = ctx.ww.GetResource<FSMCondTables>();

		//
		const CanonicalAxis* ca = nullptr;
		AxisID ax{};
        for (std::uint16_t i = 0; i < cat.axes.size(); ++i) {
            if (cat.axes[i].axisName == "Movement") { ca = &cat.axes[i]; ax = AxisID{ i }; break; }
        }
        assert(ca && "Movement axis not found. Check axis name & registration");

        const auto tv = MakeTransitionView(*ca);
        const auto& ct = ctab.byAxis[ax.id];

        // State name → StateID
        auto sid_idle = StateID{ static_cast<uint32_t>(std::lower_bound(
            ca->stateNames.begin(), ca->stateNames.end(), std::string("Idle")) - ca->stateNames.begin()) };
        auto sid_move = StateID{ static_cast<uint32_t>(std::lower_bound(
            ca->stateNames.begin(), ca->stateNames.end(), std::string("Move")) - ca->stateNames.begin()) };

        EvalCtx eval{}; // いまは空でOK

        // case1: canMove=true, wantMove=true => Idle -> Move
        Game::Character::FSM::Movement::MovementEnv env1{};
        env1.canMove = true; env1.wantMove = true;
        {
            auto d = TransitionSelector(sid_idle, tv, ct, env1, eval);
            std::cout << "[Idle->?] changed=" << d.changed << " to=" << d.to.id << "\n";
            assert(d.changed && d.to.id == sid_move.id);
        }

        // case2: canMove=true, wantMove=false => Move -> Idle
        Game::Character::FSM::Movement::MovementEnv env2{};
        env2.canMove = true; env2.wantMove = false;
        {
            auto d = TransitionSelector(sid_move, tv, ct, env2, eval);
            std::cout << "[Move->?] changed=" << d.changed << " to=" << d.to.id << "\n";
            assert(d.changed && d.to.id == sid_idle.id);
        }

	}
}