#include "Game/Debug/Public/DebugApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/FSM/Public/Core/Types.hpp"

#include "Engine/FSM/Public/Core/Analysis.hpp"

#include "Engine/FSM/Public/Core/AxisLookup.hpp"

#include "Game/Character/Private/FSM/Private/Movement/MovementEnv.hpp"

#include "Game/Character/Private/FSM/Private/Movement/Public/MovementFieldReader.hpp"

#include <iostream>

namespace Game::FSM::Debug
{
    using namespace Engine::FSM::Core;


 //   static std::uint32_t findIdx(const std::vector<std::string>& v, std::string_view n) {
 //       auto it = std::find(v.begin(), v.end(), n);
 //       return it == v.end() ? UINT32_MAX : (uint32_t)std::distance(v.begin(), it);
 //   }



	//void SmokeTest_Movement(Engine::WorldSystem::Core::WorldCtx& ctx)
	//{
	//	auto& cat = ctx.ww.GetResource<FSMCatalog>();
	//	auto& db = ctx.ww.GetResource<AxisRuntimeDB>();

 //       AxisRuntime* rt = db.get("Movement");
 //       if (!rt || !rt->canon) { std::printf("no Movement runtime\n"); return; }
 //       const CanonicalAxis& ax = *rt->canon;
 //       if (ax.fsms.empty()) { std::printf("no Movement FSM\n"); return; }
 //       const CanonicalFSM& f = ax.fsms.front(); // Basic

 //       const uint32_t sIdle = findIdx(ax.stateOrder, "Idle");
 //       const uint32_t sMoving = findIdx(ax.stateOrder, "Moving");
 //       const uint32_t slotTr = findIdx(ax.slotOrder, "Transition");
 //       const uint32_t slotSt = findIdx(ax.slotOrder, "Stop");
 //       const uint32_t profDef = findIdx(ax.profileOrder, "Default");

 //       Game::Character::FSM::Movement::MovementEnv env{};
 //       EvalCtx ectx{ 0 };

 //       env.hasMoveInput = true; env.canMove = true;
 //       auto d1 = DecideNext_BySingleSlot(f, *rt, sIdle, profDef, slotTr, env, ectx);
 //       std::printf("Idle +Transition => to=%u changed=%d (expect Moving=%u)\n", d1.to, d1.changed, sMoving);

 //       env.hasMoveInput = false; env.canMove = true;
 //       auto d2 = DecideNext_BySingleSlot(f, *rt, sMoving, profDef, slotSt, env, ectx);
 //       std::printf("Moving +Stop     => to=%u changed=%d (expect Idle=%u)\n", d2.to, d2.changed, sIdle);

	//}


    using namespace Engine::FSM::Core;
    using namespace Engine::FSM::Core::AxisLookup;
    static void PrintEvalSet(const CanonicalAxis& ax, const NextEvalSet& s)
    {
        std::cout << " slots: ";
        for (auto sl : s.slots) std::cout << ax.slotOrder[sl] << " ";
        std::cout << "\n condBits: ";
        for (auto b : s.condBits) std::cout << ax.condOrder[b] << " ";
        std::cout << "\n";
    }

    //void SmokeTest_Movement(Engine::WorldSystem::Core::WorldCtx& ctx)
    //{


    //    //auto& cat = ctx.ww.GetResource<FSMCatalog>();
    //    //auto& db = ctx.ww.GetResource<AxisRuntimeDB>();

    //    //AxisRuntime* rt = db.get("Movement");
    //    //assert(rt && rt->canon);
    //    //const CanonicalAxis& ax = *rt->canon;

    //    //// --- Lookup で取得 ---
    //    //const CanonicalFSM& fsm = ax.fsms[FindFSMIdx(ax, "Basic")];
    //    //const uint32_t profile = FindProfileIdx(ax, "Default");
    //    //const uint32_t sTransition = FindSlotIdx(ax, "Transition");
    //    //const uint32_t sStop = FindSlotIdx(ax, "Stop");
    //    //const uint32_t cCanMove = FindCondIdx(ax, "canMove");
    //    //const uint32_t cShouldStop = FindCondIdx(ax, "shouldStop");
    //    //const uint32_t stIdle = FindStateIdx(ax, "Idle");
    //    //const uint32_t stMoving = FindStateIdx(ax, "Moving");

    //    //std::vector<uint32_t> slots = { sTransition, sStop };

    //    //BitEnvSnapshot env;
    //    //env.ensureSize(ax.condOrder.size());

    //    //std::cout << "==== FSM Movement Test ====\n";

    //    //// ---------- Case 1 ----------
    //    //env.clearAll();
    //    //env.set(cCanMove, true);
    //    //env.set(cShouldStop, false);
    //    //{
    //    //    Decision d = DecideNext_Slots(fsm, stIdle, profile, slots, env);
    //    //    std::cout << "[1] Idle + canMove => " << d.to << (d.changed ? " (changed)" : "") << "\n";
    //    //    assert(d.changed && d.to == stMoving);
    //    //}

    //    //// ---------- Case 2 ----------
    //    //env.clearAll();
    //    //env.set(cCanMove, false);
    //    //env.set(cShouldStop, false);
    //    //{
    //    //    Decision d = DecideNext_Slots(fsm, stIdle, profile, slots, env);
    //    //    std::cout << "[2] Idle + no input => " << d.to << (d.changed ? " (changed)" : "") << "\n";
    //    //    assert(!d.changed && d.to == stIdle);
    //    //}

    //    //// ---------- Case 3 ----------
    //    //env.clearAll();
    //    //env.set(cCanMove, false);
    //    //env.set(cShouldStop, true);
    //    //{
    //    //    Decision d = DecideNext_Slots(fsm, stMoving, profile, slots, env);
    //    //    std::cout << "[3] Moving + shouldStop => " << d.to << (d.changed ? " (changed)" : "") << "\n";
    //    //    assert(d.changed && d.to == stIdle);
    //    //}

    //    //// ---------- Case 4 ----------
    //    //env.clearAll();
    //    //env.set(cCanMove, true);
    //    //env.set(cShouldStop, true);
    //    //{
    //    //    Decision d = DecideNext_Slots(fsm, stMoving, profile, slots, env);
    //    //    std::cout << "[4] Moving + both true => " << d.to << (d.changed ? " (changed)" : "") << "\n";
    //    //    assert(d.to == stIdle || d.to == stMoving);
    //    //}

    //    //std::cout << "==== All OK ====\n";

    //    auto& db = ctx.ww.GetResource<AxisRuntimeDB>();
    //    AxisRuntime* rt = db.get("Movement");
    //    assert(rt && rt->canon);
    //    const CanonicalAxis& ax = *rt->canon;

    //    const CanonicalFSM& fsm = ax.fsms[FindFSMIdx(ax, "Basic")];
    //    const uint32_t profile = FindProfileIdx(ax, "Default");

    //    const uint32_t sTransition = FindSlotIdx(ax, "Transition");
    //    const uint32_t sStop = FindSlotIdx(ax, "Stop");

    //    const uint32_t cCanMove = FindCondIdx(ax, "canMove");
    //    const uint32_t cShouldStop = FindCondIdx(ax, "shouldStop");

    //    const uint32_t stIdle = FindStateIdx(ax, "Idle");
    //    const uint32_t stMoving = FindStateIdx(ax, "Moving");

    //    // ---- build whole-table plans
    //    PrecomputedEvalPlans plans = BuildEvalPlans(fsm);

    //    // ---- case: Idle / Default
    //    {
    //        NextEvalSet set = GetNextEvalSet(plans, stIdle, profile);
    //        std::cout << "[EvalPlan] Idle/Default\n";
    //        PrintEvalSet(ax, set);

    //        // candidateSlots must be exactly {Transition}
    //        assert(set.slots.size() == 1);
    //        assert(set.slots[0] == sTransition);

    //        // condBits must be exactly {canMove}
    //        assert(set.condBits.size() == 1);
    //        assert(set.condBits[0] == cCanMove);
    //    }

    //    // ---- case: Moving / Default
    //    {
    //        NextEvalSet set = GetNextEvalSet(plans, stMoving, profile);
    //        std::cout << "[EvalPlan] Moving/Default\n";
    //        PrintEvalSet(ax, set);

    //        // candidateSlots must be exactly {Transition, Stop} (優先順: Transitionが先)
    //        assert(set.slots.size() == 2);
    //        assert(set.slots[0] == sTransition);
    //        assert(set.slots[1] == sStop);

    //        // condBits must be exactly {canMove, shouldStop} （昇順ユニーク）
    //        assert(set.condBits.size() == 2);
    //        assert(set.condBits[0] == cCanMove);
    //        assert(set.condBits[1] == cShouldStop);
    //    }


    //    std::cout << "==== EvalPlan Movement: All OK ====\n";

    //}

    void SmokeTest_Movement(Engine::WorldSystem::Core::WorldCtx& ctx)
    {
        auto& cat = ctx.ww.GetResource<FSMCatalog>();
        auto& db = ctx.ww.GetResource<AxisRuntimeDB>();

        AxisRuntime* rt = db.get("Movement");
        assert(rt && rt->canon);
        const CanonicalAxis& ax = *rt->canon;

        const CanonicalFSM& fsm = ax.fsms[ax.fsmIdxByName.at("Basic")];
        const uint32_t profile = ax.profileIdxByName.at("Default");
        const uint32_t stIdle = ax.stateIdxByName.at("Idle");
        const uint32_t stMoving = ax.stateIdxByName.at("Moving");

        // 1) 前計算テーブル
        PrecomputedEvalPlans plans = BuildEvalPlans(fsm);

        // 2) リーダ / ビットバッファ
        Game::Character::FSM::Movement::MovementFieldReader reader{};
        BitEnvSnapshot      bits;
        bits.ensureSize((uint32_t)ax.condOrder.size());

        auto run_case = [&](const char* label, uint32_t from, float inputMag) {
            // a) 必要Condを取得（状態とプロファイルから）
            NextEvalSet eval = GetNextEvalSet(plans, from, profile);

            // b) 入力設定 → 必要ビットだけ派生実行
            reader.movementInputMag = inputMag;
            bits.clearAll();
            ExecuteDerivedCondsFiltered(rt->plan, eval.condBits, reader, bits);

            // c) Decide：候補スロットだけ渡す
            Decision d = DecideNext_Slots(fsm, from, profile, eval.slots, bits);
            printf("%s: input=%.2f  %u -> %u  %s\n",
                label, inputMag, from, d.to, d.changed ? "(changed)" : "");
            return d;
            };

        // ------- 期待：0.30 で Idle→Moving, 0.00 で Moving→Idle
        auto d1 = run_case("[Idle]   canMove true", stIdle, 0.30f);
        assert(d1.changed && d1.to == stMoving);

        auto d2 = run_case("[Idle]   canMove false", stIdle, 0.00f);
        assert(!d2.changed && d2.to == stIdle);

        auto d3 = run_case("[Moving] shouldStop true", stMoving, 0.00f);
        assert(d3.changed && d3.to == stIdle);

        // 両方trueケース（0.30だと canMove=true, shouldStop=false）
        // Moving からは Transition(自己遷移 prio=1) と Stop(prio=0) が候補
        // → prio=1 の自己遷移が勝つことを確認
        auto d4 = run_case("[Moving] both? -> self", stMoving, 0.30f);
        assert(d4.to == stMoving);

        puts("==== Runtime Movement: All OK ====");

    }
}