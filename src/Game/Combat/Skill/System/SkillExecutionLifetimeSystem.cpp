#include "SkillExecutionLifetimeSystem.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMStates.hpp"
#include "Game/Combat/Skill/Component/SkillExecutionComponent.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillStateComponent.hpp"

#include "Game/Combat/Skill/FSM/SkillStateTags.hpp"

#include "Engine/ECS/EntityUtils/EntityUtils.h"

void Game::Combat::Skill::System::UpdateSkillExecutionLifetimeSystem(eNsECS::EntityMgr& ecs)
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::FSM;

	for (eNsECS::Entity eSkill : ecs.view<SkillExecutionComponent>())
	{
		const auto& exec = ecs.get<SkillExecutionComponent>(eSkill);
		const auto& caster = exec.caster;

		if (!ecs.isAlive(caster))
		{
			eNsECS::EntityUtils::MarkForPendingDestroy(ecs, eSkill);
			continue;
		}

		if (!ecs.hasComponent<SkillStateComponent>(caster)) continue;
		const auto& state = ecs.get<SkillStateComponent>(caster);

		// FSM Ç™ NoneÇ…ëJà⁄ÇµÇΩÇÁçÌèú
		if (state.current == StateTag::NONE)
		{
			eNsECS::EntityUtils::MarkForPendingDestroy(ecs, eSkill);
			std::cout << "[SkillExecutionLifetimeSystem.cpp]: phase complete -> destroy " << eSkill.id << "\n";
		}
	}

}