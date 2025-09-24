#pragma once


#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#include "Game/Character/FSM/Interference/Core/Data/FSMInterferenceRequest.hpp"

#include "Game/Character/FSM/CC/Effect/StateScoped/Handler/IStateEffectHandler.hpp"
#include "Game/Character/FSM/Movement/API/External/MovementInterferenceRequest.hpp"
#include "Game/Character/FSM/CC/CCStateTags.hpp"

#include "Game/Character/FSM/Movement/MovementStateTags.hpp"

namespace Game::Character::FSM::CC::StateEffect
{
	struct emitMovementInterferenceOnStunned :public IStateEffectHandler
	{
		float sec;

		explicit emitMovementInterferenceOnStunned(float s) : sec(s) {}

		void execute(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity e,
			const Game::Character::FSM::CC::StateModel::CCFSMContext& ctx) override
		{
			using namespace Game::Character::FSM::Movement::API::External;
			namespace IF = Game::Character::FSM::Interference::Core::Data;
			namespace CC = Game::Character::FSM::CC;
			namespace Mv = Game::Character::FSM::Movement;

			// Movement 側の req Component の取得
			auto* reqComp = tryGetMovementInterferenceRequestComponent(ecs, e);
			// もしnullptr なら (それはないようにするべきだが）
			if (!reqComp) return;

			IF::FSMInterferenceRequest
				req(CC::AxisTag::CCAxis, Mv::AxisTag::MovementAxis);

			req.forcedState = Mv::StateTag::IDLE;
			// TODO: 情報入力は多様化するときにAPI化で対応
			req.severity = IF::ControlSeverity::High;
			req.mode = IF::InterferenceMode::ForceTransition;
			req.durationSec = 5.0f;// 干渉残り時間 = CC 効果時間 but now ctx does't have total cc time
			req.issuerEntity = e;// これいる？
			req.targetEntity = e;// これいる？

			if (req.durationSec <= 0.0f) return;// 無効だから無視

			reqComp->requests.push_back(std::move(req));
		}
	};
}

