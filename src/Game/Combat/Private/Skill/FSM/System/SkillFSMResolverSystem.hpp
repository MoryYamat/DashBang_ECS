#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Private/Skill/MasterData/SkillDatabase.h"
#include "Game/Combat/Private/Skill/Component/SkillExecutionContextComponent.hpp"

#include "Game/Combat/Private/Skill/Def/SkillDef.hpp"
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Private/Skill/FSM/Effect/Hook/SkillEffectHook.hpp"


#include "Game/Combat/Private/Skill/FSM/StateModel/SkillStateComponent.hpp"
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMLeaseComponent.hpp"

// Reset
#include "Game/Combat/Private/Skill/FSM/Statemodel/SkillFSMDefinition.hpp"

#include <typeindex>

namespace Game::Combat::Skill::FSM::System
{

	class SkillFSMResolverSystem
	{
	public:
		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);

		//static void tryTriggerEffect(
		//	const SkillEffectHook& hook,
		//	Engine::ECS::EntityMgr& ecs,
		//	Engine::ECS::Entity eExec,
		//	Engine::ECS::Entity caster,
		//	const SkillDef& def,
		//	const SkillFSMContext& ctx,
		//	std::type_index current,
		//	std::type_index previous
		//);

	private:

		static void tryTriggerEffect(
			Engine::ECS::EntityMgr& ecs,
			const Game::Combat::Skill::FSM::Effect::SkillEffectHook& hook,
			const Engine::ECS::Entity caster,
			const Game::Combat::Skill::Def::SkillDef& def,
			const Game::Combat::Skill::FSM::SkillFSMContext& ctx,
			const std::type_index& current,
			const std::type_index& previous
		);
		
		static void tryTriggerReset
		(
			Engine::ECS::EntityMgr& ecs,
			const Engine::ECS::Entity caster,
			const Game::Combat::Skill::FSM::SkillFSMDefinition& fsm,
			const Game::Combat::Skill::Def::SkillDef& def,
			const Game::Combat::Skill::FSM::SkillFSMContext& ctx,
			const std::type_index& current,
			const std::type_index& previous
		);

		static bool tryApplyForcedTransition
		(
			Engine::ECS::EntityMgr& ecs,
			const Engine::ECS::Entity e,
			Game::Combat::Skill::FSM::StateModel::SkillStateComponent& state,
			const Game::Combat::Skill::FSM::StateModel::SkillFSMLeaseComponent& lease,
			Game::Combat::Skill::Component::SkillExecutionContextComponent& exec
		);

		static bool applyStateUpdate
		(
			Game::Combat::Skill::FSM::StateModel::SkillStateComponent& state,
			std::type_index to,
			Game::Combat::Skill::Component::SkillExecutionContextComponent& exec
		);

		static void runSkillEffects
		(
			Engine::ECS::EntityMgr& ecs,
			const Engine::ECS::Entity e,
			Game::Combat::Skill::FSM::StateModel::SkillStateComponent& state,
			Game::Combat::Skill::Component::SkillExecutionContextComponent& exec
		);
	};
}