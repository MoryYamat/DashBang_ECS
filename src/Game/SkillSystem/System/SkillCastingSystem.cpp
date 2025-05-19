// 

#include "SkillCastingSystem.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Game/SkillSystem/Component/SkillInstanceComponent.h"

#include "Game/SkillSystem/MasterData/SkillDefinition.h"

void SkillSystem::Casting::SpawnSkillHitArea(ECS& ecs, SkillDatabase& skillDB)
{
	for (Entity e : ecs.view<SkillInstanceComponent, Logic2DTransformComponent>())
	{
		auto& skillInstance = ecs.get<SkillInstanceComponent>(e);

		// 初回発動時のみ
		if (skillInstance.timeSinceCast > 0.0f)
			continue;

		std::cout << "[SkillCastingSystem] skillId: " << skillInstance.skillId << std::endl;

		const SkillDefinition& def = skillDB.Get(skillInstance.skillId);
		const auto& logic = ecs.get<Logic2DTransformComponent>(skillInstance.caster);

		// world 変換
		glm::vec2 worldCenter = logic.positionXZ;
		Attack2DShape shape;

		if (def.shape.IsCircle())
		{
			Circle2DAttack circle = def.shape.AsCircle();
			circle.center += worldCenter;
			shape = Attack2DShape{ circle };
		}
		else if (def.shape.IsSector())
		{
			Sector2DAttack sector = def.shape.AsSector();
			sector.center += worldCenter;
			sector.direction = logic.front;
			shape = Attack2DShape{ sector };
		}

		// 攻撃範囲エンティティ生成
		Entity attack = ecs.createEntity();
		Attack2DAreaComponent area;
		area.shape = shape;
		area.lifetime = def.duration;
		area.owner = skillInstance.caster;
		area.skillEntity = e;

		ecs.addComponent(attack, area);
	}
}

void SkillSystem::Casting::SkillLifetimeSystem()
{

}
