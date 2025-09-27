#include "MovementSpeedModifier.hpp"

#include "Game/Combat/Skill/Component/SkillExecutionContextComponent.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillStateComponent.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillFSMStates.hpp"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Game/Common/Logic/FSM/Query/FSMQuery.hpp"

#include <iostream>

float Game::Combat::Skill::FSM::Modifier::Movement::CalcMovementSpeedMultiplierFromSkillFSM
(
	eNsECS::EntityMgr& ecs,
	eNsECS::Entity entity
)
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Common::Logic::FSM::Query;
	using namespace Game::Combat::Skill::FSM;

	using namespace Game::Combat::Skill::Database;

	FSMQuery fsmQuery(entity, ecs);

	auto phaseOpt = fsmQuery.getCurrentState<StateModel::SkillStateComponent>();

	if (!phaseOpt.has_value()) return 1.0f;

	if (!ecs.hasComponent<SkillExecutionContextComponent>(entity)) return 1.0f;// 現時点ではSkillExecutionComponentはスキル専用のEntityに付与されている

	// std::cout << "here\n";
	const auto& exec = ecs.get<SkillExecutionContextComponent>(entity);
	const auto& db = ecs.getResource<SkillDatabase>();
	if (!db.Has(exec.skillId)) return 1.0f;

	const auto& entry = db.Get(exec.skillId);

	if (!entry.def.movementModifiers.has_value()) return 1.0f;

	const auto& modifiers = entry.def.movementModifiers.value();
	auto it = modifiers.movementSpeedMultiplier.find(*phaseOpt);// std::optional->中身にアクセス
	if (it != modifiers.movementSpeedMultiplier.end()) 
	{
		return it->second;
	}

	return 1.0f;
}