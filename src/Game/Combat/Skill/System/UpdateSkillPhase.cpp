#include "UpdateSkillPhase.h"

#include "Game/Combat/Skill/System/SkillCastingSystem.h"
#include "Game/Combat/Skill/System/UpdateSkillLifetimes.h"

#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"

#include "Engine/ECS/Component/Tags/PendingDestroyComponent.h"

#include "Engine/ECS/EntityUtils/EntityUtils.h"

void Game::Combat::Skill::System::UpdateSkillPhase(eNsECS::EntityMgr& ecs, float deltaTime, gNsSkillData::SkillDatabase& skillDB)
{
	for (eNsECS::Entity e : ecs.view<gNsSkillComp::SkillInstanceComponent>())
	{
		auto& instance = ecs.get<gNsSkillComp::SkillInstanceComponent>(e);
		const auto& def = skillDB.Get(instance.skillId);

		instance.timeSinceCast += deltaTime;

		switch (instance.phase)
		{
		case gNsSkillComp::SkillPhase::Casting:
			if (instance.timeSinceCast >= def.castTime)
			{
				instance.phase = gNsSkillComp::SkillPhase::Active;
				instance.timeSinceCast = 0.0f;// ƒŠƒZƒbƒg
				gNsSkillSystem::SpawnSkillHitArea(ecs, skillDB);// UŒ‚”ÍˆÍ¶¬
			}
			break;

		case gNsSkillComp::SkillPhase::Active:
			if (instance.timeSinceCast >= def.duration)
			{
				instance.phase = gNsSkillComp::SkillPhase::Recovery;
				instance.timeSinceCast = 0.0f;
			}
			break;

		case gNsSkillComp::SkillPhase::Recovery:
			if (instance.timeSinceCast >= def.recoveryTime)
			{
				instance.phase = gNsSkillComp::SkillPhase::Completed;
			}
			break;

		case gNsSkillComp::SkillPhase::Completed:
			// «««ŒÃ‚ÌŠÖ”(íœ—\’è)«««
			// SkillSystem::Lifetime::CleanUpCompletedSkills(ecs);
			std::cout << "[UpdateSkillPhase.cpp(Completed Skill)] entity id " << instance.skillId << std::endl;
			
			eNsECS::EntityUtils::MarkForPendingDestroyWithChildren(ecs, e, instance.spawnedHitAreas);
			
			// addPendingDestroyComp(ecs, e, instance);

			break;
		}


	}
}

