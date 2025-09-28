#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Game/Combat/Skill/Def/SkillDef.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMContext.hpp"
#include "Game/Combat/Skill/FSM/Effect/Hook/SkillEffectHook.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillStateComponent.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMLeaseComponent.hpp"

// Reset
#include "Game/Combat/Skill/FSM/Statemodel/SkillFSMDefinition.hpp"

#include <typeindex>

namespace Game::Combat::Skill::FSM::System
{

	class SkillFSMResolverSystem
	{
	public:
		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);

		//static void tryTriggerEffect(
		//	const SkillEffectHook& hook,
		//	eNsECS::EntityMgr& ecs,
		//	eNsECS::Entity eExec,
		//	eNsECS::Entity caster,
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
			Game::Combat::Skill::FSM::StateModel::SkillFSMLeaseComponent& lease,
			Game::Combat::Skill::Database::SkillDatabase& db
		);

		static bool applyStateUpdate
		(
			Game::Combat::Skill::FSM::StateModel::SkillStateComponent& state,
			std::type_index to
		);

		static void runSkillEffects
		(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity e,
			Game::Combat::Skill::FSM::StateModel::SkillStateComponent& state,
			Game::Combat::Skill::Database::SkillDatabase& db
		);
	};
}