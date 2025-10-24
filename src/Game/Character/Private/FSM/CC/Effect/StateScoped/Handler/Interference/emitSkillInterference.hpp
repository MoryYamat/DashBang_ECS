#pragma once
#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"


#include "Game/Character/Private/FSM/CC/StateModel/CCFSMContext.hpp"

#include "Game/Character/Private/FSM/CC/Effect/StateScoped/Handler/IStateEffectHandler.hpp"

#include "Game/Combat/Private/Skill/API/Public/tryGetSkillInterferenceRequestComponent.hpp"


#include "Game/Character/Private/FSM/CC/CCStateTags.hpp"
#include "Game/Combat/Private/Skill/FSM/SkillStateTags.hpp"

namespace Game::Character::FSM::CC::StateEffect
{
	struct emitSkillInterference : IStateEffectHandler
	{
		float sec;

		explicit emitSkillInterference(float sec) : sec(sec) {}

		void execute(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity e,
			const Game::Character::FSM::CC::StateModel::CCFSMContext& ctx) override
		{

			namespace IF = Game::Character::FSM::Interference::Core::Data;			
			namespace CC = Game::Character::FSM::CC;

			namespace SkillFSM = Game::Combat::Skill::FSM;

			auto* reqComp = Game::Combat::Skill::API::External::tryGetSkillInterferenceRequestComponent(ecs, e);

			if (!reqComp) return;

			IF::FSMInterferenceRequest
				req(CC::AxisTag::CCAxis, SkillFSM::AxisTag::SKILLAXIS);

			req.forcedState = SkillFSM::StateTag::INTERRUPTED;
			// TODO: 情報入力は多様化するときにAPI化で対応
			req.severity = IF::ControlSeverity::High;
			req.mode = IF::InterferenceMode::ForceTransition;
			req.durationSec = sec;
			req.issuerEntity = e;// 不要の可能性あり
			req.targetEntity = e;// 不要の可能性あり

			if(req.durationSec <= 0.0f) return;// 無効だから無視

			reqComp->requests.push_back(std::move(req));
		}
	};
}