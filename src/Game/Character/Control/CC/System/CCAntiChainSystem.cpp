#include "CCAntiChainSystem.hpp"

#include "Engine/Time/WorldClock.hpp"

#include "Game/Character/Control/CC/Policy/CCAntiChainPolicyDatabase.hpp"

#include "Game/Character/Control/CC/Component/CCAntiChainComponent.hpp"
#include "Game/Character/FSM/CC/StateModel/CCStateComponent.hpp"
#include "Game/Character/FSM/CC/StateModel/CCFSMTransitionRequestComponent.hpp"

#include "Game/Character/FSM/CC/CCStateTags.hpp"

#include <iostream>

namespace Game::Character::Control::CC
{
	using namespace Engine::Time;

	using namespace Game::Character::Control::CC::Policy;
	using namespace Game::Character::Control::CC::Component;

	using namespace Game::Character::FSM::CC;
	using namespace Game::Character::FSM::CC::StateModel;

	void CCAntiChainSystem(EntityMgr& ecs)
	{
		// 時刻とポリシー取得
		const auto& clock = ecs.getResource<WorldClockData>();
		const float now = clock.now;

		const auto& pdb = ecs.getResource<CCAntiChainPolicyDatabase>();
		const auto& policy = pdb.ResolveForFSM("basic");// ひとまず固定

		for (auto e : ecs.view<
			CCStateComponent,
			CCAntiChainComponent,
			CCFSMTransitionRequestComponent
		>())
		{
			auto& state = ecs.get<CCStateComponent>(e);
			auto& antiChain = ecs.get<CCAntiChainComponent>(e);
			auto& reqs = ecs.get<CCFSMTransitionRequestComponent>(e);

			// A. IMMUNE の期限切れ監視(毎フレーム)
			if (antiChain.immune && now >= antiChain.immuneUntil)
			{
				if (state.current == StateTag::IMMUNE)
				{
					if (!reqs.hasExactRequest(StateTag::NONE, 100000))
					{
						reqs.requests.push_back({ .requestedTo = StateTag::NONE, .priority = 100000 });
						std::cout << "[AntiChain] IMMUNE expired -> request NONE" << " /clock = " << now << "\n";
					}
				}
				else
				{
					antiChain.immune = false;
				}
			}

			// B. 遷移イベントが来たフレームだけ処理
			const auto& ev = state.applied;
			if (!ev.isValid) continue;
			if (state.transitionSerial <= antiChain.lastHandledSerial) continue;

			// 一度だけ処理
			antiChain.lastHandledSerial = state.transitionSerial;

			// 
			const bool fromNeutral = (ev.fromState == StateTag::NONE) || (ev.fromState == StateTag::IMMUNE);
			const bool toCC = (ev.toState != StateTag::NONE) && (ev.toState != StateTag::IMMUNE);
			const bool fromCC = (ev.fromState != StateTag::NONE) && (ev.fromState != StateTag::IMMUNE);
			const bool toNONE = (ev.toState == StateTag::NONE);

			// TODO: 同フレーム内の複数CCをどう扱うかは課題！！！！！
			if (toCC)
			{
				// 初段なら無条件でアンカー
				if (antiChain.count == 0) {
					antiChain.windowStart = ev.appliedAt;

					// リセット
					antiChain.clearImmune();
					antiChain.disarmImmune();
				}
				else
				{
					// WindowSec内かどうか確認
					const float dt = ev.appliedAt - antiChain.windowStart;
					if (dt >= policy.windowSec)
					{
						// window 外なら カウントリセット/windowリセット
						antiChain.count = 0;
						antiChain.windowStart = ev.appliedAt;

						antiChain.clearImmune();
						antiChain.disarmImmune();


						std::cout << "[AntiChain] window reset at=" << ev.appliedAt
							<< " dt=" << dt << " ws=" << policy.windowSec << "\n";
					}
				}

				antiChain.count += policy.weightOf(ev.toState);
				// std::cout << "[CCAntiChainSystem]: antiChain.count " << antiChain.count << "\n";
			}



			if (antiChain.count >= policy.threshold)
			{
				antiChain.immuneArmed = true;
			}


			// armed 状態なら　終了直後にIMMUNEをリクエスト
			if (fromCC && toNONE && antiChain.immuneArmed)
			{
				if (!reqs.hasExactRequest(StateTag::IMMUNE, 0))
				{
					reqs.requests.push_back({ .requestedTo = StateTag::IMMUNE, .priority = 10000 });
					std::cout << "[AntiChain] IMMUNE expired -> request IMMUNE" 
						<< " /clock = " << ev.appliedAt 
						<< " /release time: " << ev.appliedAt + policy.immunitySec << "\n";
				}

				antiChain.immune = true;
				antiChain.immuneUntil = ev.appliedAt + policy.immunitySec;
				// antiChain.resetWindow(ev.appliedAt);
			}



			//// IMMUNE 満了チェック
			//if (state.current == StateTag::IMMUNE)
			//{
			//	if (now >= antiChain.immuneUntil)
			//	{
			//		// 具体的なリクエスト挙動は今後変更する可能性あり
			//		if (!reqs.hasExactRequest(StateTag::NONE, 0))
			//		{
			//			reqs.requests.push_back({ .requestedTo = StateTag::NONE, .priority = 10000 });
			//			std::cout << "[AntiChain] IMMUNE expired -> request NONE" << " /clock = " << now <<"\n";
			//		}
			//		antiChain.clearImmune();// 厳密にやるなら実遷移完了時に消す運用も可
			//	}
			//	continue;// IMMUNE 中はストライクを数えない
			//}


			//// 連鎖ウィンドウの期限切れを毎フレームで掃除（怠惰リセットに加えて即時リセットも）
			//if ((state.current == StateTag::NONE) &&
			//	(now - antiChain.windowStart) > policy.windowSec)
			//{
			//	antiChain.resetWindow(now);
			//}


			//// FIXME: count>thresholdを判定していない
			//// CCが終わってNONEに戻った瞬間に,count>thresholdなら IMMUNE 適用
			//{
			//	const bool fromCC = (state.previous != StateTag::NONE) && (state.previous != StateTag::IMMUNE);
			//	const bool toNONE = (state.current == StateTag::NONE);

			//	if (fromCC && toNONE && antiChain.immuneArmed)
			//	{
			//		if (!reqs.hasExactRequest(StateTag::IMMUNE, 0))
			//		{
			//			reqs.requests.push_back({ .requestedTo = StateTag::IMMUNE, .priority = 0 });
			//		}
			//		antiChain.immune = true;
			//		antiChain.immuneUntil = now + policy.immunitySec;
			//		antiChain.resetWindow(now);
			//		antiChain.disarmImmune();
			//	}
			//}
			//
			//// 中立(NONE/IMMUNE)->CCに入った瞬間を検出しストライク計上
			//{            
			//	const bool fromNeutral = (state.previous == StateTag::NONE) || (state.previous == StateTag::IMMUNE);
			//	const bool toCC = (state.current != StateTag::NONE) && (state.current != StateTag::IMMUNE);


			//	// 毎フレーム true && true -> X
			//	// １フレーム true && true -> その後 false true 
			//	if (fromNeutral && toCC)
			//	{
			//		// 同フレーム二重計上ガード（必要なら ε 比較にする）
			//		if (antiChain.count > 0 && antiChain.lastStrikeAt == now)
			//		{
			//			continue;
			//		}
			//		antiChain.lastStrikeAt = now;

			//		// 連鎖ウィンドウ外ならリセット
			//		if ((now - antiChain.windowStart) > policy.windowSec)
			//		{
			//			antiChain.resetWindow(now);
			//		}

			//		// FIXME: CC中毎フレーム加算されてしまっている
			//		// FIXME: CCが適用された瞬間に一回だけ加算されなければならない

			//		// CC種ごとの重みで加算
			//		antiChain.count += policy.weightOf(state.current);

			//		std::cout << "[CCAntiChainSystem]: antiChain.count " << antiChain.count << "\n";

			//		// 閾値到達 -> IMMUNEをリクエストして期限セット
			//		if (antiChain.count >= policy.threshold)
			//		{
			//			////
			//			//if (!reqs.hasExactRequest(StateTag::IMMUNE, 0))
			//			//{
			//			//	reqs.requests.push_back({ .requestedTo = StateTag::IMMUNE, .priority = 0 });
			//			//	// std::cout << "[AntiChain] threshold hit -> request IMMUNE (" << policy.immunitySec << "s)\n";
			//			//}

			//			//antiChain.immune = true;
			//			//antiChain.immuneUntil = now + policy.immunitySec;

			//			antiChain.immuneArmed = true;
			//			// antiChain.resetWindow(now);
			//		}
			//	}
			//}
		}
	}
}

