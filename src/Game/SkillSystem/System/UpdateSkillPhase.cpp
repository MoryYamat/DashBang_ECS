#include "UpdateSkillPhase.h"

#include "Game/SKillSystem/System/SkillCastingSystem.h"
#include "Game/SkillSystem/System/UpdateSkillLifetimes.h"

#include "Game/SkillSystem/Component/SkillInstanceComponent.h"

#include "Core/ECS/Component/Tags/PendingDestroyComponent.h"

#include "Core/ECS/EntityUtils/EntityUtils.h"

void SkillSystem::Phase::UpdateSkillPhase(ECS& ecs, float deltaTime, SkillDatabase& skillDB)
{
	for (Entity e : ecs.view<SkillInstanceComponent>())
	{
		auto& instance = ecs.get<SkillInstanceComponent>(e);
		const auto& def = skillDB.Get(instance.skillId);

		instance.timeSinceCast += deltaTime;

		switch (instance.phase)
		{
		case SkillPhase::Casting:
			if (instance.timeSinceCast >= def.castTime)
			{
				instance.phase = SkillPhase::Active;
				instance.timeSinceCast = 0.0f;// ƒŠƒZƒbƒg
				SkillSystem::Casting::SpawnSkillHitArea(ecs, skillDB);// UŒ‚”ÍˆÍ¶¬
			}
			break;

		case SkillPhase::Active:
			if (instance.timeSinceCast >= def.duration)
			{
				instance.phase = SkillPhase::Recovery;
				instance.timeSinceCast = 0.0f;
			}
			break;

		case SkillPhase::Recovery:
			if (instance.timeSinceCast >= def.recoveryTime)
			{
				instance.phase = SkillPhase::Completed;
			}
			break;

		case SkillPhase::Completed:
			// «««ŒÃ‚ÌŠÖ”(íœ—\’è)«««
			// SkillSystem::Lifetime::CleanUpCompletedSkills(ecs);
			std::cout << "[UpdateSkillPhase.cpp(Completed Skill)] entity id " << instance.skillId << std::endl;
			
			EntityUtils::MarkForPendingDestroyWithChildren(ecs, e, instance.spawnedHitAreas);
			
			// addPendingDestroyComp(ecs, e, instance);

			break;
		}


	}
}

