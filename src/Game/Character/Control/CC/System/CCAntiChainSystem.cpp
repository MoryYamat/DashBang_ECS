#include "CCAntiChainSystem.hpp"

#include "Engine/Time/WorldClock.hpp"

#include "Game/Character/Control/CC/Policy/CCAntiChainPolicyDatabase.hpp"

#include "Game/Character/Control/CC/Component/CCAntiChainComponent.hpp"
#include "Game/Character/FSM/CC/StateModel/CCStateComponent.hpp"
#include "Game/Character/FSM/CC/StateModel/CCFSMTransitionRequestComponent.hpp"

#include "Game/Character/FSM/CC/CCStateTags.hpp"

namespace Game::Character::Control::CC
{
	using namespace Engine::Time;

	using namespace Game::Character::Control::CC::Policy;
	using namespace Game::Character::Control::CC::Component;

	using namespace Game::Character::FSM::CC;
	using namespace Game::Character::FSM::CC::StateModel;

	void CCAntiChainSystem(EntityMgr& ecs, float deltaTime)
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

			// IMMUNNE 満了チェック
			if (state.current == StateTag::IMMUNE)
			{
				if (now >= antiChain.immuneUntil)
				{
					// 具体的なリクエスト挙動は今後変更する可能性あり
					if (!reqs.hasExactRequest(StateTag::NONE, 0))
					{
						reqs.requests.push_back({ .requestedTo = StateTag::NONE, .priority = 0 });
						// std::cout << "[AntiChain] IMMUNE expired -> request NONE\n";
					}
					antiChain.clearImmune();// 厳密にやるなら実遷移完了時に消す運用も可
				}
				continue;// IMMUNE 中はストライクを数えない
			}


			// 連鎖ウィンドウの期限切れを毎フレームで掃除（怠惰リセットに加えて即時リセットも）
			if ((state.current == StateTag::NONE) &&
				(now - antiChain.windowStart) > policy.windowSec)
			{
				antiChain.resetWindow(now);
			}

			// 中立(NONE/IMMUNE)->CCに入った瞬間を検出
			const bool fromNeutral = (state.previous == StateTag::NONE) || (state.previous == StateTag::IMMUNE);
			const bool toCC = (state.current != StateTag::NONE) && (state.current != StateTag::IMMUNE);

			if (fromNeutral && toCC)
			{            
				// 同フレーム二重計上ガード（必要なら ε 比較にする）
				if (antiChain.count > 0 && antiChain.lastStrikeAt == now)
				{
					continue;
				}
				antiChain.lastStrikeAt = now;

				// 連鎖ウィンドウ外ならリセット
				if ((now - antiChain.windowStart) > policy.windowSec)
				{
					antiChain.resetWindow(now);
				}

				// CC種ごとの重みで加算
				antiChain.count += policy.weightOf(state.current);

				// 閾値到達 -> IMMUNEをリクエストして期限セット
				if (antiChain.count >= policy.threshold)
				{
					if (!reqs.hasExactRequest(StateTag::IMMUNE, 0))
					{
						reqs.requests.push_back({ .requestedTo = StateTag::IMMUNE, .priority = 0 });
						// std::cout << "[AntiChain] threshold hit -> request IMMUNE (" << policy.immunitySec << "s)\n";
					}
					antiChain.immune = true;
					antiChain.immuneUntil = now + policy.immunitySec;
					antiChain.resetWindow(now);
				}

			}
		}
	}
}
