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

    //void SmokeTest_Movemet(Engine::WorldSystem::Core::WorldCtx& ctx)
    //{
    //	auto& cat = ctx.ww.GetResource<FSMCatalog>();// 実行時参照: 遷移ペア
    //	auto& ctab = ctx.ww.GetResource<FSMCondTables>();// 実行時参照: 遷移条件

    //	//
    //	const CanonicalAxis* ca = nullptr;
    //	AxisID ax{};
 //       for (std::uint16_t i = 0; i < cat.axes.size(); ++i) {
 //           if (cat.axes[i].axisName == "Movement") { ca = &cat.axes[i]; ax = AxisID{ i }; break; }
 //       }
 //       assert(ca && "Movement axis not found. Check axis name & registration");

 //       const auto tv = MakeTransitionView(*ca);
 //       const auto& ct = ctab.byAxis[ax.id];

 //       // State name → StateID
 //       auto sid_idle = StateID{ static_cast<uint32_t>(std::lower_bound(
 //           ca->stateNames.begin(), ca->stateNames.end(), std::string("Idle")) - ca->stateNames.begin()) };
 //       auto sid_move = StateID{ static_cast<uint32_t>(std::lower_bound(
 //           ca->stateNames.begin(), ca->stateNames.end(), std::string("Move")) - ca->stateNames.begin()) };

 //       EvalCtx eval{}; // いまは空でOK

 //       // case1: canMove=true, wantMove=true => Idle -> Move
 //       Game::Character::FSM::Movement::MovementEnv env1{};
 //       env1.canMove = true; env1.wantMove = true;
 //       {
 //           auto d = TransitionSelector(sid_idle, tv, ct, env1, eval);
 //           std::cout << "[Idle->?] changed=" << d.changed << " to=" << d.to.id << "\n";
 //           assert(d.changed && d.to.id == sid_move.id);
 //       }

 //       // case2: canMove=true, wantMove=false => Move -> Idle
 //       Game::Character::FSM::Movement::MovementEnv env2{};
 //       env2.canMove = true; env2.wantMove = false;
 //       {
 //           auto d = TransitionSelector(sid_move, tv, ct, env2, eval);
 //           std::cout << "[Move->?] changed=" << d.changed << " to=" << d.to.id << "\n";
 //           assert(d.changed && d.to.id == sid_idle.id);
 //       }

    //}

    void SmokeTest_Movemet(Engine::WorldSystem::Core::WorldCtx& ctx)
    {
        using namespace Engine::FSM::Core;

        auto& cat = ctx.ww.GetResource<FSMCatalog>();        // 遷移グラフ
        auto& profs = ctx.ww.GetResource<FSMCondProfiles>();   // CondTable プロファイル集

        // Movement 軸の特定
        const CanonicalAxis* ca = nullptr;
        AxisID ax{};
        for (std::uint16_t i = 0; i < cat.axes.size(); ++i) {
            if (cat.axes[i].axisName == "Movement") { ca = &cat.axes[i]; ax = AxisID{ i }; break; }
        }
        assert(ca && "Movement axis not found. Check axis name & registration");

        const auto tv = MakeTransitionView(*ca);

        // State name → StateID
        const auto sid_idle = StateID{ static_cast<uint32_t>(std::lower_bound(
            ca->stateNames.begin(), ca->stateNames.end(), std::string("Idle")) - ca->stateNames.begin()) };
        const auto sid_move = StateID{ static_cast<uint32_t>(std::lower_bound(
            ca->stateNames.begin(), ca->stateNames.end(), std::string("Move")) - ca->stateNames.begin()) };

        // CondTable 解決（profileId が無いときは 0 にフォールバック）
        auto resolveCt = [&](CondProfileID profileId) -> const CondTable& {
            const auto& perAxis = profs.byAxis[ax.id];
            if (auto it = perAxis.find(profileId); it != perAxis.end())
                return it->second;              // 存在: そのプロファイルを使う
            return perAxis.at(0);               // 不存在: 既定(0)へフォールバック（Build時に保証済）
            };

        EvalCtx eval{}; // いまは空でOK

        // ===== Default(0) の期待テスト =====
        {
            const auto& ct0 = resolveCt(0);

            // case1: canMove=true, wantMove=true => Idle -> Move
            Game::Character::FSM::Movement::MovementEnv env1{};
            env1.canMove = true; env1.wantMove = true;
            {
                auto d = TransitionSelector(sid_idle, tv, ct0, env1, eval);
                std::cout << "[Default][Idle->?] changed=" << d.changed << " to=" << d.to.id << "\n";
                assert(d.changed && d.to.id == sid_move.id);
            }

            // case2: canMove=true, wantMove=false => Move -> Idle
            Game::Character::FSM::Movement::MovementEnv env2{};
            env2.canMove = true; env2.wantMove = false;
            {
                auto d = TransitionSelector(sid_move, tv, ct0, env2, eval);
                std::cout << "[Default][Move->?] changed=" << d.changed << " to=" << d.to.id << "\n";
                assert(d.changed && d.to.id == sid_idle.id);
            }
        }

        // ===== 不存在プロファイル → 既定(0)へ安全フォールバック =====
        {
            const CondProfileID missing = 99999;
            const auto& ct_fb = resolveCt(missing);
            const auto& ct0 = resolveCt(0);

            // 期待：挙動が default と同じになる
            Game::Character::FSM::Movement::MovementEnv env{};
            env.canMove = true; env.wantMove = true;

            auto d0 = TransitionSelector(sid_idle, tv, ct0, env, eval);
            auto dfb = TransitionSelector(sid_idle, tv, ct_fb, env, eval);
            std::cout << "[Fallback][" << missing << "][Idle->?] changed=" << dfb.changed
                << " to=" << dfb.to.id << "\n";
            assert(d0.changed == dfb.changed && d0.to.id == dfb.to.id);
        }

        // ===== 任意の既存 SkillID プロファイル（存在するなら）も軽く実行 =====
        // 例: 123 をビルドしている場合に挙動確認（期待値はテーブル次第なので assert はしない）
        {
            const CondProfileID skillId = 123;
            const auto& perAxis = profs.byAxis[ax.id];
            if (perAxis.find(skillId) != perAxis.end()) {
                const auto& ct = resolveCt(skillId);
                Game::Character::FSM::Movement::MovementEnv env{};
                env.canMove = true; env.wantMove = true;

                auto d = TransitionSelector(sid_idle, tv, ct, env, eval);
                std::cout << "[Skill " << skillId << "][Idle->?] changed=" << d.changed
                    << " to=" << d.to.id << "\n";
                // ※ここは仕様に応じて assert を足してOK（たとえば skill 用に条件が反転している等）
            }
        }

    }

}