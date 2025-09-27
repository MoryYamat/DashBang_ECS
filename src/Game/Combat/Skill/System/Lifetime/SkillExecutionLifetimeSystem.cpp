#include "SkillExecutionLifetimeSystem.hpp"

#include "Game/Combat/Skill/FSM/StateModel/SkillFSMStates.hpp"
#include "Game/Combat/Skill/Component/SkillExecutionContextComponent.hpp"
#include "Game/Combat/Skill/FSM/StateModel/SkillStateComponent.hpp"

#include "Game/Combat/Skill/FSM/SkillStateTags.hpp"

#include "Engine/ECS/EntityUtils/EntityUtils.h"


// 削除予定：SkillExecutionをcharacterアクターへ付与する方式に変更したため
void Game::Combat::Skill::System::UpdateSkillExecutionLifetimeSystem(eNsECS::EntityMgr& ecs)
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::Combat::Skill::FSM;
	using namespace Game::Combat::Skill::FSM::StateModel;

	for (eNsECS::Entity eSkill : ecs.view<SkillExecutionContextComponent>())
	{
		const auto& exec = ecs.get<SkillExecutionContextComponent>(eSkill);
		const auto& caster = exec.caster;

		if (!ecs.isAlive(caster))
		{
			eNsECS::EntityUtils::MarkForPendingDestroy(ecs, eSkill);
			continue;
		}

		if (!ecs.hasComponent<SkillStateComponent>(caster)) continue;
		const auto& state = ecs.get<SkillStateComponent>(caster);

		// FSM が Noneに遷移 かつ SkillExecutionComponentを持つエンティティが存在するならば削除
		if (state.current == StateTag::NONE)
		{
			eNsECS::EntityUtils::MarkForPendingDestroy(ecs, eSkill);
			std::cout << "[SkillExecutionLifetimeSystem.cpp]: phase complete -> destroy " << eSkill.id << "\n";
		}
	}

}