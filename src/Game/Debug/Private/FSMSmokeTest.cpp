#include "Game/Debug/Public/DebugApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/FSM/Public/Base/Types.hpp"

#include "Game/Character/Private/FSM/Private/Movement/MovementEnv.hpp"

namespace Game::FSM::Debug
{
    static std::uint32_t findIdx(const std::vector<std::string>& v, std::string_view n) {
        auto it = std::find(v.begin(), v.end(), n);
        return it == v.end() ? UINT32_MAX : (uint32_t)std::distance(v.begin(), it);
    }


	using namespace Engine::FSM::Base;

	void SmokeTest_Movemet(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& cat = ctx.ww.GetResource<FSMCatalog>();
		auto& db = ctx.ww.GetResource<AxisRuntimeDB>();

        AxisRuntime* rt = db.get("Movement");
        if (!rt || !rt->canon) { std::printf("no Movement runtime\n"); return; }
        const CanonicalAxis& ax = *rt->canon;
        if (ax.fsms.empty()) { std::printf("no Movement FSM\n"); return; }
        const CanonicalFSM& f = ax.fsms.front(); // Basic

        const uint32_t sIdle = findIdx(ax.stateOrder, "Idle");
        const uint32_t sMoving = findIdx(ax.stateOrder, "Moving");
        const uint32_t slotTr = findIdx(ax.slotOrder, "Transition");
        const uint32_t slotSt = findIdx(ax.slotOrder, "Stop");
        const uint32_t profDef = findIdx(ax.profileOrder, "Default");

        Game::Character::FSM::Movement::MovementEnv env{};
        EvalCtx ectx{ 0 };

        env.hasMoveInput = true; env.canMove = true;
        auto d1 = DecideNext_BySingleSlot(f, *rt, sIdle, profDef, slotTr, env, ectx);
        std::printf("Idle +Transition => to=%u changed=%d (expect Moving=%u)\n", d1.to, d1.changed, sMoving);

        env.hasMoveInput = false; env.canMove = true;
        auto d2 = DecideNext_BySingleSlot(f, *rt, sMoving, profDef, slotSt, env, ectx);
        std::printf("Moving +Stop     => to=%u changed=%d (expect Idle=%u)\n", d2.to, d2.changed, sIdle);

	}
}